#pragma once
#include "../thread/thread.h"
#include <atomic>

//采用one thread per loop 每个线程只能有一个EventLoop实体

class Channel;
class EventLoop :public noncopyable
{
public:
	EventLoop();
	~EventLoop();

	void loop();

	void updateChannel(Channel* channel);

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


private:
	void abortNotInLoopThread();

	std::atomic<bool> m_looping;//是否在loop
	std::atomic<bool> m_quit;//是否退出loop
	bool m_eventHandling;//是否正在处理event事件

	const pid_t t_threadId;
};

