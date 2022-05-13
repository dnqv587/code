#pragma once
#include "../base/noncopyable.h"
#include <pthread.h>
#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <assert.h>
#ifdef _WIN32
#include <windows.h>
static pid_t gettid()
{
	return ::GetCurrentThreadId();//获取当前线程id
}
#else
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <linux/unistd.h>

#if !__GLIBC_PREREQ(2,30)//glibc
static pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}
#endif
#endif


//CurrentThread类，存储当前线程的信息
class  CurrentThread :public noncopyable
{
	friend class ThreadData;
	friend class ThreadInitializer;//提供访问name和tid的权限
	friend void afterFork();//访问tid
public:
	static pid_t tid()//线程ID
	{
		if (t_tid == 0)
		{
			t_tid = ::gettid();
		}
		return t_tid;
	}
	static std::string name()//线程名
	{
		return t_name;
	}
	static bool isMainThread()//是否为主线程
	{
		return t_tid == ::gettid();
	}
private:
	thread_local static std::string t_name;//当前线程名
	thread_local static pid_t t_tid;//当前线程ID
};



void afterFork()//fork进程后将子进程的主线程重新初始化
{
	CurrentThread::t_tid = 0;
	CurrentThread::t_name = "main";
	CurrentThread::tid();
}

class ThreadInitializer //初始化类，用来设置主线程名、线程id
{
public:
	ThreadInitializer()
	{
		CurrentThread::t_name = "main";
		CurrentThread::tid();
		pthread_atfork(NULL, NULL, afterFork);//注册fork回调函数
	}
};



//线程函数,线程中运行的函数
class ThreadData
{
	typedef std::function<void()> ThreadFunc;
public:
	ThreadData(const ThreadFunc& func, const std::string& name, const std::shared_ptr<pid_t>& tid)
		:m_func(func),
		m_name(name),
		wkTid(tid)
	{}

	//在当前线程中运行函数
	void run()
	{
		pid_t tid = CurrentThread::tid();
		std::shared_ptr<pid_t> pTid = wkTid.lock();//判断线程是否被销毁
		if (pTid)
		{
			*pTid = tid;
			pTid.reset();//?
		}

		CurrentThread::t_name = m_name.empty() ? "Thread" : m_name;
		::prctl(PR_SET_NAME, CurrentThread::t_name);//设置线程名

		m_func();//运行回调函数
		CurrentThread::t_name = "finished";//线程结束

	}

private:
	ThreadFunc m_func;
	std::string m_name;
	std::weak_ptr<pid_t> wkTid;//使用weak_ptr是防止在使用前被其他线程析构了
};

//线程回调函数
static void* thread(void* arg)
{
	ThreadData* data = static_cast<ThreadData*> (arg);
	data->run();//运行函数
	delete data;
	return NULL;
}

class Thread:public noncopyable
{
public:
	typedef std::function<void()> ThreadFunc;

	explicit Thread(const ThreadFunc& func, const std::string& name = std::string())
		:m_func(func),
		m_name(name),
		m_isStarted(false),
		m_tid(0),
		m_isJoined(false),
		m_pTid(new pid_t(0))
	{
		++g_threadNum;
	}
	~Thread() 
	{
		if (m_isStarted && !m_isJoined)
		{
			pthread_detach(m_tid);
		}
	}

	//创建线程
	void start();

	//回收线程
	void join();

	//线程是否在运行
	bool isStarted() { return m_isStarted; }

	//线程名
	std::string& name() { return m_name; }

private:

	pthread_t m_tid;//线程id
	std::shared_ptr<pid_t> m_pTid;//线程标识，判断线程是否被销毁
	ThreadFunc m_func;
	std::string m_name;
	bool m_isStarted;//该线程是否在运行
	bool m_isJoined;//该线程是否已被回收

	static std::atomic<int> g_threadNum;//记录线程数量
};


void Thread::start()
{
	assert(!m_isStarted);
	m_isStarted = true;
	ThreadData* data = new ThreadData(m_func, m_name, m_pTid);
	if (pthread_create(&m_tid, NULL, thread, data))
	{
		m_isStarted = false;
		delete data;
		abort();//退出程序
	}
}

void Thread::join()
{
	assert(m_isStarted);
	assert(!m_isJoined);
	m_isJoined = true;
	pthread_join(m_tid, NULL);
}

thread_local std::string CurrentThread::t_name = "unnamedThread";
thread_local pid_t CurrentThread::t_tid = 0;

ThreadInitializer init;//使用全局变量初始化主线程

std::atomic<int> Thread::g_threadNum;