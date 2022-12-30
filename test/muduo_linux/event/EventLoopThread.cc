#include "EventLoopThread.h"
#include "EventLoop.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, const std::string& name)
	:m_loop(nullptr),
	m_exiting(false),
	m_thread(std::bind(&EventLoopThread::threadFunc, this), name),
	m_mutex(),
	m_cond(m_mutex),
	m_callback(cb)
{

}

EventLoopThread::~EventLoopThread()
{
	m_exiting = true;
	if (m_loop)
	{
		m_loop->quit();
		m_thread.join();
	}
}

EventLoop* EventLoopThread::startLoop()
{
	assert(!m_thread.isStarted());
	m_thread.start();
	EventLoop* loop = nullptr;

	{
		MutexLockGuard lock(m_mutex);
		while (m_loop = nullptr)
		{
			m_cond.wait();
		}
		loop = m_loop;
	}
	return loop;
}

void EventLoopThread::threadFunc()
{
	EventLoop loop;
	if (m_callback)
	{
		m_callback(&loop); 
	}
	{
		MutexLockGuard lock(m_mutex);
		m_loop = &loop;
		m_cond.notify();
	}
	loop.loop();

	{
		MutexLockGuard lock(m_mutex);
		m_loop = nullptr;
	}

}
