#pragma once
#include "noncopyable.h"
#include <pthread.h>
/*
��װmutex
*/

class MutexLock:public noncopyalbe
{
public:
	MutexLock() 
	{
		pthread_mutex_init(&m_mutex, NULL);
	}
	~MutexLock()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	void lock()
	{
		pthread_mutex_lock(&m_mutex);
	}
	void unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}
	pthread_mutex_t* getMutexLockPtr()
	{
		return &m_mutex;
	}

private:
	pthread_mutex_t m_mutex;
};

/*
RAII����
*/
class MutexLockGuard
{
public:
	MutexLockGuard(MutexLock& mutex):m_MutexLock(mutex)
	{
		m_MutexLock.lock();
	}
	~MutexLockGuard()
	{
		m_MutexLock.unlock();
	}
private:
	MutexLock& m_MutexLock;
};