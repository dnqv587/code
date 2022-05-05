#pragma once
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include"mutex.h"
#include "../base/noncopyable.h"

/*
条件变量对于RAII技法封装
*/
class Condition:public noncopyable
{
public:
	explicit Condition(MutexLock& mutex) :m_mutex(mutex) 
	{ 
		pthread_cond_init(&m_cond, NULL);
	}
	~Condition()
	{
		pthread_cond_destroy(&m_cond);
	}

	void wait()
	{
		pthread_cond_wait(&m_cond, m_mutex.getMutexLockPtr());
	}
	
	//在指定时间内条件变量阻塞当前线程
	bool waitTime(int seconds)
	{
		struct timespec abstime;
#ifdef _WIN32
		abstime.tv_sec = static_cast<long>(time(NULL) + seconds);
		abstime.tv_nsec = 0;
#else
		clock_gettime(CLOCK_REALTIME, &abstime);
		abstime.tv_sec += seconds;
#endif
		return ETIMEDOUT == pthread_cond_timedwait(&m_cond, m_mutex.getMutexLockPtr(), &abstime);
	}

	//通知一个阻塞在当前条件变量的线程
	void notify()
	{
		pthread_cond_signal(&m_cond);
	}

	//通知全部阻塞在当前条件变量的线程
	void notifyAll()
	{
		pthread_cond_broadcast(&m_cond);
	}


private:
	MutexLock& m_mutex;
	pthread_cond_t m_cond;
};
