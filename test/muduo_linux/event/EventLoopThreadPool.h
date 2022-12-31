#pragma once
#include "../base/noncopyable.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

class EventLoop;
class EventLoopThread;
class EventLoopThreadPool :noncopyable
{
public:
	using ThreadInitCallback = std::function<void(EventLoop*)>;

	EventLoopThreadPool(EventLoop* baseLoop,const std::string& name);

	~EventLoopThreadPool();

	/// <summary>
	/// 设置线程数
	/// </summary>
	/// <param name="numThreads"></param>
	void setThreadNum(int numThreads)
	{
		m_numThreads = numThreads;
	}

	/// <summary>
	/// 开启loop线程
	/// </summary>
	/// <param name="cb"></param>
	void start(const ThreadInitCallback& cb);

	/// <summary>
	/// 依次获取loop
	/// </summary>
	/// <returns></returns>
	EventLoop* getNextLoop();

private:
	EventLoop* m_baseLoop;
	std::string m_name;
	bool m_started;
	int m_numThreads;
	int m_next;
	std::vector<std::unique_ptr<EventLoopThread>> m_threads;
	std::vector<EventLoop*> m_loops;
};

