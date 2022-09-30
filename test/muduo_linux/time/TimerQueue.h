#pragma once
#include "../base/noncopyable.h"
#include "../base/Callbacks.h"
#include "Timestamp.h"
#include "Timer.h"
#include "../event/Channel.h"
#include <set>
#include <memory>
#include <vector>
#include <utility>

//计时器队列
class TimerID;
class EventLoop;
class TimerQueue :noncopyable
{
public:
	TimerQueue(EventLoop* loop);

	//添加一个计时器--只能在IO线程调用--负责转发
	TimerID addTimer(const TimerCallback& cb, Timestamp when, double interval);

	void cancel(TimerID timerID);

	int getFd()
	{
		return m_timerfd;
	}
private:
	using Entry = std::pair<Timestamp, Timer*>;
	using TimerList = std::set<Entry>;
	typedef std::pair<Timer*, int64_t> ActiveTimer;
	typedef std::set<ActiveTimer> ActiveTimerSet;

	//修改定时器列表
	void addTimerInLoop(Timer* timer);
	//当timerfd alarms进行调用
	void handleRead();
	//移除已到期时间的Timer，并返回过期Timer容器
	std::vector<std::pair<Timestamp, Timer*>> getExpired(Timestamp now);
	void reset(const std::vector<Entry>& expired, Timestamp now);
	bool insert(Timer* timer);

	EventLoop* m_loop;//所属loop
	const int m_timerfd;
	Channel m_timerfdChannel;
	TimerList m_timers;//Timer容器，根据expiration(执行时间)进行排序
	//for cancel()
	ActiveTimerSet m_activeTimers;//未过期的Timer
	bool m_callingExpiredTimers;
	ActiveTimerSet m_cancelingTimer;//撤销的Timer;

};

