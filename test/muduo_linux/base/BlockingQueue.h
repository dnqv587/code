#pragma once

#include "../thread/mutex.h"
#include "../thread/condition.h"
#include "noncopyable.h"

#include <queue>
#include <assert.h>

//��������
template<class T>
class BlokingQueue :public noncopyable
{
public:
	BlokingQueue() :m_lock(), m_queue(), m_waitNotEmpty(m_lock)
	{
	}
	~BlokingQueue()
	{
	}

	//����---֪ͨ������take�ϵ��߳�
	void put(const T& val)
	{
		MutexLockGuard lock(m_lock);
		m_queue.emplace(std::move(val));
		m_waitNotEmpty.notify();
	}

	//��ȡ---����Ϊ����ȴ�
	T take()
	{
		MutexLockGuard lock(m_lock);
		while (m_queue.empty())
		{
			m_waitNotEmpty.wait();
		}
		assert(!m_queue.empty());

		T&& front(std::move(m_queue.front()));
		m_queue.pop();
		return front;
	}

	//���
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

	//��С
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


private:
	mutable MutexLock m_lock;
	Condition m_waitNotEmpty GUARDED_BY(m_lock);//�ȴ����зǿ�
	std::queue<T> m_queue GUARDED_BY(m_lock);//�������ݳ�Ա�ܸ������ܱ����������ݵĶ�ȡ������Ҫ������ʣ���д�������Ҫ��ռ����
};

