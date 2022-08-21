#pragma once
#include "../base/noncopyable.h"
#include "../event/EventLoop.h"
#include "../base/Callbacks.h"
#include "../event/Channel.h"
#include <set>

//计时器队列
class TimerID;
class Timer;
class TimerQueue :noncopyable
{
public:
	TimerQueue(EventLoop* loop);

	//添加一个计时器
	TimerID addTimer(const TimerCallback& cb, Timestamp when, double interval);

	void cancel(TimerID timerID);

private:
	using Entry = std::pair<Timestamp, std::unique_ptr<Timer>>;
	using TimerList = std::set<Entry>;
	void handleRead();
	//移除已到期时间的Timer，并返回过期Timer容器
	std::vector<Entry> getExpired(Timestamp now);
	void reset(const std::vector<Entry>& expired, Timestamp now);
	bool insert(Timer* timer);

	EventLoop* m_loop;//所属loop
	const int m_timerfd;
	Channel m_timerfdChannel;
	TimerList m_timers;//Timer容器，根据expiration(执行时间)进行排序
};

