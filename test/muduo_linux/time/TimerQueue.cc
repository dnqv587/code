#include "TimerQueue.h"
#include "Timer.h"
#include "TimerID.h"
#include "../event/EventLoop.h"
#include "../logger/logging.h"
#include <utility>
#include <sys/timerfd.h>
#include <unistd.h>


namespace detail
{
	//创建timerfd
	int createTimerfd()
	{
		int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
		if (timerfd < 0)
		{
			LOG_SYSFATAL << "Failed int timer_create!";
		}
		return timerfd;
	}
	//将定时时间转换为timespec结构体
	struct timespec howMuchTimeFromNow(Timestamp when)
	{
		int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
		if (microseconds < 100)
		{
			microseconds = 100;
		}
		struct timespec ts;
		ts.tv_sec = static_cast<time_t>(microseconds / ::kMicroSecondsPerSecond);
		ts.tv_nsec = static_cast<long>((microseconds % ::kMicroSecondsPerSecond) * 1000);
		return ts;
	}

	//初始化timerfd
	void resetTimerfd(int timerfd, Timestamp expiration)
	{
		struct itimerspec newValue;
		struct itimerspec oldValue;
		memZero(&newValue, sizeof newValue);
		memZero(&oldValue, sizeof oldValue);
		newValue.it_value = howMuchTimeFromNow(expiration);
		int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
		if (ret)
		{
			LOG_SYSERR << "timerfd_settime()";
		}
	}

	//从timerfd中读取8个字节
	void readTimerfd(int timerfd, Timestamp now)
	{
		uint64_t howmany;
		ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
		LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
		if (n != sizeof howmany)
		{
			LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
		}
	}

}



TimerQueue::TimerQueue(EventLoop* loop)
	:m_loop(loop),
	m_timerfd(detail::createTimerfd()),
	m_timerfdChannel(m_loop, m_timerfd),
	m_timers(),
	m_callingExpiredTimers(false)
{
	m_timerfdChannel.setReadCallback(std::bind(&TimerQueue::handleRead, this));
	
	m_timerfdChannel.enableReading();//监听读事件
}

TimerID TimerQueue::addTimer(const TimerCallback& cb, Timestamp when, double interval)
{
	Timer* timer = new Timer(cb, when, interval);
	m_loop->runInLoop(std::bind(&TimerQueue::addTimerInLoop,this,timer));
	return TimerID(timer, timer->sequence());

}

void TimerQueue::addTimerInLoop(Timer* timer)
{
	m_loop->assertInLoopThread();
	bool earliestChanged = this->insert(timer);

	if (earliestChanged)
	{
		detail::resetTimerfd(m_timerfd, timer->expiration());
	}
}


void TimerQueue::cancel(TimerID timerID)
{
	m_loop->runInLoop(std::bind(&TimerQueue::cancelInLoop, this, timerID));
}

void TimerQueue::cancelInLoop(TimerID timerID)
{
	m_loop->assertInLoopThread();
	assert(m_timers.size() == m_activeTimers.size());
	ActiveTimer timer(timerID.m_timer, timerID.m_sequence);
	auto iter = m_activeTimers.find(timer);//查找此TimerID的ActiveTimer
	if (iter != m_activeTimers.end())
	{
		ssize_t n = m_timers.erase(std::pair<Timestamp, Timer*>(iter->first->expiration(), iter->first));
		assert(n == 1); (void)n;
		delete iter->first;
		m_activeTimers.erase(iter);
	}
	else if (m_callingExpiredTimers)//正在调用ExpiredTimers
	{
		m_cancelingTimer.insert(timer);//先将此Timer存入m_cancelingTimer
	}
	assert(m_timers.size() == m_activeTimers.size());
}

void TimerQueue::handleRead()
{
	m_loop->assertInLoopThread();
	Timestamp now(Timestamp::now());
	detail::readTimerfd(m_timerfd, now);//读取内容

	std::vector<Entry> expired = this->getExpired(now);//移除过期Timer
	
	m_callingExpiredTimers = true;//保持原子性
	m_cancelingTimer.clear();
	//调用过期的timer
	for (const auto& it : expired)
	{
		it.second->run();
	}
	m_callingExpiredTimers = false;

	this->reset(expired, now);//将过期的Timer重新初始化
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
	assert(m_timers.size() == m_activeTimers.size());
	std::vector<Entry> expired;
	//哨兵,UINTPTR_MAX:64bit机器的最大地址
	Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator iter = m_timers.lower_bound(sentry);//查找第一个不小于sentry的迭代器,又因为Timer*为UINTPTR_MAX即最大值，所以取得的iter为大于Timestamp的迭代器
	assert(iter == m_timers.end() || now < iter->first);//当Timestamp相等时会比较Timer*
	std::copy(m_timers.begin(), iter, std::back_inserter(expired));
	m_timers.erase(m_timers.begin(), iter);
	//std::move(m_timers.begin(), iter, std::back_inserter(expired));
	for (const auto& it : expired)
	{
		ActiveTimer timer(it.second, it.second->sequence());
		size_t n = m_activeTimers.erase(timer);
		assert(n == 1); (void)n;
	}
	assert(m_timers.size() == m_activeTimers.size());
	return expired;
}


void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
	Timestamp nextExpire;
	
	for (const auto& it : expired)
	{
		ActiveTimer timer(it.second, it.second->sequence());
		//当此Timer是repeat时则重置时间并重新插入
		if (it.second->repeat() && m_cancelingTimer.find(timer) == m_cancelingTimer.end())
		{
			it.second->restart(now);
			this->insert(it.second);
		}
		else //否则，直接删除此Timer
		{
			delete it.second;
		}
	}
	//将timerfd定时为下一个时间
	if (!m_timers.empty())
	{
		nextExpire = m_timers.begin()->second->expiration();//获取下一个时间
	}
	if (nextExpire.valid())
	{
		detail::resetTimerfd(m_timerfd, nextExpire);
	}
}

bool TimerQueue::insert(Timer* timer)
{
	m_loop->assertInLoopThread();
	assert(m_timers.size() == m_activeTimers.size());
	bool earliestChanged = false;
	Timestamp when = timer->expiration();//获取timer运行时间
	auto it = m_timers.cbegin();
	if (it == m_timers.cend() || when < m_timers.begin()->first)
	{
		earliestChanged = true;
	}
	{
		std::pair<TimerList::iterator, bool> result = m_timers.insert(Entry(when, timer));
		assert(result.second);
		(void)result;//防止编译时告警，当断言检查关闭时，result编标不会再被使用
	}
	{
		std::pair<ActiveTimerSet::iterator, bool> result = m_activeTimers.insert(ActiveTimer(timer, timer->sequence()));
		assert(result.second); (void)result;
	}
	assert(m_timers.size() == m_activeTimers.size());
	return earliestChanged;

}
