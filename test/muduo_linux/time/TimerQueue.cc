#include "TimerQueue.h"
#include "Timer.h"
#include "TimerID.h"
#include "../event/EventLoop.h"
#include "../logger/logging.h"
#include <utility>
#include <sys/timerfd.h>


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

}



TimerQueue::TimerQueue(EventLoop* loop)
	:m_loop(loop),
	m_timerfd(detail::createTimerfd()),
	m_timerfdChannel(m_loop, m_timerfd)
{

}

TimerID TimerQueue::addTimer(const TimerCallback& cb, Timestamp when, double interval)
{
	Timer* timer = new Timer(cb, when, interval);
	m_loop->runInLoop(std::bind(&TimerQueue::addTimerInLoop,this,timer));
	return TimerID(timer, timer->sequence());

}

void TimerQueue::cancel(TimerID timerID)
{

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

void TimerQueue::handleRead()
{

}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
	std::vector<Entry> expired;
	//哨兵,UINTPTR_MAX:64bit机器的最大地址
	Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator iter = m_timers.lower_bound(sentry);//查找第一个不小于sentry的迭代器,又因为Timer*为UINTPTR_MAX即最大值，所以取得的iter为大于Timestamp的迭代器
	assert(iter == m_timers.end() || now < iter->first);//当Timestamp相等时会比较Timer*
	std::copy(m_timers.begin(), iter, std::back_inserter(expired));
	m_timers.erase(m_timers.begin(), iter);
	//std::move(m_timers.begin(), iter, std::back_inserter(expired));

	return expired;
}


void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{

}

bool TimerQueue::insert(Timer* timer)
{
	m_loop->assertInLoopThread();
	assert(m_timers.size() == m_activeTimers.size());
	bool earliestChanged = false;
	Timestamp when = timer->expiration();//获取timer运行时间
	if (m_timers.empty() || when < m_timers.begin()->first)
	{
		earliestChanged = true;
	}
	{
		std::pair<TimerList::iterator, bool> result = m_timers.insert(Entry(when, timer));
		assert(result.second);
		(void)result;
	}
	{
		std::pair<ActiveTimerSet::iterator, bool> result = m_activeTimers.insert(ActiveTimer(timer, timer->sequence()));
		assert(result.second); (void)result;
	}
	assert(m_timers.size() == m_activeTimers.size());
	return earliestChanged;

}
