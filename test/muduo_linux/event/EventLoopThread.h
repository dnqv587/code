#pragma once
#include "../base/noncopyable.h"
#include "../thread/condition.h"
#include "../thread/mutex.h"
#include "../thread/thread.h"

class EventLoop;
class EventLoopThread :noncopyable 
{
public:
	using ThreadInitCallback = std::function<void(EventLoop*)>;

	EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(), const std::string& name = std::string());

	~EventLoopThread();

	/// <summary>
	/// 开启Loop线程 
	/// </summary>
	/// <returns>loop对象--为loop线程中的栈对象</returns>
	EventLoop* startLoop();

private:
	/// <summary>
	/// Loop线程
	/// </summary>
	void threadFunc();

	EventLoop* m_loop GUARDED_BY(m_mutex);
	bool m_exiting;
	Thread m_thread;
	MutexLock m_mutex;
	Condition m_cond GUARDED_BY(m_mutex);
	ThreadInitCallback m_callback;
};