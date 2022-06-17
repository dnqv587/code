#pragma once
#include "../base/BlockingQueue.h"
#include "thread.h"
#include <vector>
#include <atomic>

class ThreadPool :private noncopyable
{
public:
	enum
	{
		CORE_NUM
	};
	typedef std::function<void()> Task;
	ThreadPool(const char* name = "ThreadPool");
	~ThreadPool();

	void start(unsigned int numThreads = CORE_NUM);

	void stop();

	void run(Task task);

private:
	void thread();

	BlockingQueue<ThreadPool::Task> m_TaskQueue;//任务队列
	std::vector<std::unique_ptr<Thread>> m_threads;//线程容器
	std::atomic<bool> m_isRunning;//是否运行
	std::string m_name;//线程池名
};