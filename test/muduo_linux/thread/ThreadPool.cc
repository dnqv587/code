#include "ThreadPool.h"
#include <unistd.h>

ThreadPool::ThreadPool(const char* name /*= "ThreadPool"*/)
	:m_name(name),
	m_isRunning(false)
	//m_TaskQueue()
{

}

ThreadPool::~ThreadPool()
{
	if (m_isRunning)
	{
		this->stop();
	}
}

void ThreadPool::start(unsigned int numThreads /*= CORE_NUM*/)
{
	assert(m_threads.empty());
	m_isRunning = true;
	m_threads.reserve(numThreads);
	//获取cpu个数
	if (numThreads == CORE_NUM)
	{
		numThreads = sysconf(_SC_NPROCESSORS_CONF);
	}
	for (int i = 0; i < numThreads; ++i)
	{
		m_threads.emplace_back(new Thread(std::bind(&ThreadPool::thread, this), m_name + std::to_string(i)));
		m_threads[i]->start();
	}

}



void ThreadPool::stop()
{
	m_isRunning = false;
	m_TaskQueue.over();
	for (const auto& t : m_threads)
	{
		t->join();
	}
}

void ThreadPool::run(Task task)
{
	if (m_threads.empty())
	{
		task();
	}
	else
	{
		if (!m_isRunning)
		{
			return;
		}
		m_TaskQueue.put(task);
	}
}

void ThreadPool::thread()
{
	while (m_isRunning)
	{
		ThreadPool::Task task(m_TaskQueue.take(COPY));

		if (task)
		{
			task();
		}
	}

}

