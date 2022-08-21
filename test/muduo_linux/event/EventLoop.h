#pragma once
#include "../thread/thread.h"
#include "../base/Callbacks.h"
#include <atomic>
#include <vector>
#include <memory>

//采用one thread per loop 每个线程只能有一个EventLoop实体

class Channel;
class Poller;
class TimerID;
class TimerQueue;
class EventLoop :public noncopyable
{
	
public:
	EventLoop();
	~EventLoop();

	void loop();
	//更新和维护Poller
	void updateChannel(Channel* channel);

	void quit()
	{
		m_quit = true;
	}

	//断言当前线程是否为IO线程，否则abort
	void assertInLoopThread()
	{
		if (!isInLoopThread())
		{
			this->abortNotInLoopThread();
		}
	}

	//当前线程是否是IO线程
	bool isInLoopThread() const 
	{
		return CurrentThread::tid() == this->t_threadId;
	}
	//获取当前线程的EventLoop对象
	EventLoop* getEventLoopOfCurrentThread();

	//定时运行函数
	TimerID runAt(const Timestamp time, const TimerCallback& cb);
	TimerID runAfter(double delay, const TimerCallback& cb);
	TimerID runEvery(double interval, const TimerCallback& cb);


private:
	void abortNotInLoopThread();

	using ChannelList = std::vector<Channel*>;
	

	std::atomic<bool> m_looping;//是否在loop
	std::atomic<bool> m_quit;//是否退出loop
	bool m_eventHandling;//是否正在处理event事件
	std::unique_ptr<Poller> m_poller;//poller对象
	ChannelList m_activeChannels;//活跃channel集合
	const pid_t t_threadId;//线程ID
	std::unique_ptr<TimerQueue> m_timerQueue;//定时器队列
};

