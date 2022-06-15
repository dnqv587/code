#pragma once

#include "../thread/mutex.h"
#include "../thread/condition.h"
#include "noncopyable.h"

#include <queue>
#include <assert.h>

//阻塞队列
template<class T>
class BlockingQueue :private noncopyable
{
public:
	BlockingQueue() :m_lock(), m_queue(), m_waitNotEmpty(m_lock), m_isRunning(true)
	{
	}
	~BlockingQueue()
	{
		m_waitNotEmpty.notifyAll();
	}

	//加入---通知阻塞在take上的线程
	
	void put(const T& val)
	{
		MutexLockGuard lock(m_lock);
		m_queue.push(std::move(val));
		m_waitNotEmpty.notify();
	}
	
	void put(T&& val)
	{
		MutexLockGuard lock(m_lock);
		m_queue.push(std::move(val));
		m_waitNotEmpty.notify();
	}

	//拿取---队列为空则等待
	T take()
	{
		MutexLockGuard lock(m_lock);
		while (m_queue.empty())
		{
			m_waitNotEmpty.wait();
		}
		if (m_isRunning)
		{
			assert(!m_queue.empty());
		}			

		T&& front(std::move(m_queue.front()));
		m_queue.pop();
		return front;
	}

	//清空
	std::queue<T> drain()
	{
		std::queue<T> que;
		{
			MutexLockGuard lock(m_lock);
			std::swap(que, m_queue);
			assert(m_queue.empty());
		}
		return que;
	}

	//大小
	size_t size()
	{
		MutexLockGuard lock(m_lock);
		return m_queue.size();
	}

	bool isEmpty()
	{
		MutexLockGuard lock(m_lock);
		return m_queue.empty();
	}

	//结束队列，并解除阻塞
	void over()
	{
		MutexLockGuard lock(m_lock);
		m_isRunning = false;
		m_waitNotEmpty.notifyAll();
	}


private:
	bool m_isRunning;
	mutable MutexLock m_lock;
	Condition m_waitNotEmpty GUARDED_BY(m_lock);//等待队列非空
	std::queue<T> m_queue GUARDED_BY(m_lock);//声明数据成员受给定功能保护。对数据的读取操作需要共享访问，而写入操作需要独占访问
};

