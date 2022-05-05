#pragma once
#include "mutex.h"
#include "condition.h"
#include "../base/noncopyable.h"

/*
����ʱ��
��;��
���̷߳��������̣߳�1�����̵߳ȴ����̶߳���ɲ�����������̲߳ż���ִ�У�2�����̶߳��ȴ����̣߳����߳���ɲ�����������߳�ִ��
*/
class CountDownLatch:public noncopyable
{
public:
	explicit CountDownLatch(int count):m_count(count), m_mutex(),m_cond(m_mutex){ }//mutexӦ����condition��ʼ��
	
	//�������������߳�
	void wait()
	{
		MutexLockGuard lock(m_mutex);
		while (m_count > 0)//��ֹ��ٻ���
		{
			m_cond.wait();
		}
	}

	//�ݼ�������������������Ϊ0���ѱ����������������߳�
	void countDown()
	{
		MutexLockGuard lock(m_mutex);
		--m_count;
		if (m_count == 0)
		{
			m_cond.notifyAll();
		}
	}

	//���ؼ���
	int getCount() const 
	{
		MutexLockGuard lock(m_mutex);
		return m_count;
	}

private:
	int m_count;//�������
	Condition m_cond;
	mutable MutexLock m_mutex;
};