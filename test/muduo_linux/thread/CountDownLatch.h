#pragma once
#include "mutex.h"
#include "condition.h"
#include "../base/noncopyable.h"

/*
倒计时门闩类
用途：
主线程发起多个子线程，1：主线程等待子线程都完成部分任务后主线程才继续执行；2：子线程都等待主线程，主线程完成部分任务后，子线程执行
*/
class CountDownLatch:public noncopyable
{
public:
	explicit CountDownLatch(int count):m_count(count), m_mutex(),m_cond(m_mutex){ }//mutex应先于condition初始化
	
	//条件变量阻塞线程
	void wait()
	{
		MutexLockGuard lock(m_mutex);
		while (m_count > 0)//防止虚假唤醒
		{
			m_cond.wait();
		}
	}

	//递减任务计数，若任务计数为0则唤醒被条件变量阻塞的线程
	void countDown()
	{
		MutexLockGuard lock(m_mutex);
		--m_count;
		if (m_count == 0)
		{
			m_cond.notifyAll();
		}
	}

	//返回计数
	int getCount() const 
	{
		MutexLockGuard lock(m_mutex);
		return m_count;
	}

private:
	int m_count;//任务计数
	Condition m_cond;
	mutable MutexLock m_mutex;
};