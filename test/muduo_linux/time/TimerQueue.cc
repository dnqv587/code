#include "TimerQueue.h"
#include "Timer.h"
#include "TimerID.h"
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

void TimerQueue::addTimerInLoop(Timer* timer)
{
	m_loop->assertInLoopThread();
	bool earliestChanged = this->insert(timer);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
	std::vector<Entry> expired;
	//哨兵
	Entry sentry = std::make_pair(now, std::unique_ptr<Timer>(reinterpret_cast<Timer*>(UINTPTR_MAX)));
	TimerList::iterator iter = m_timers.lower_bound(sentry);//查找第一个不小于sentry的迭代器
	assert(iter != m_timers.end() || now < iter->first);
	std::copy(m_timers.begin(), iter, std::back_inserter(expired));
	m_timers.erase(m_timers.begin(), iter);
	//std::move(m_timers.begin(), iter, std::back_inserter(expired));

	return expired;
}

bool TimerQueue::insert(Timer* timer)
{
	m_loop->assertInLoopThread();
	assert(m_timers.size()==
}
