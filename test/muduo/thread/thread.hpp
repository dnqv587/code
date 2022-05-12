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

#if !__GLIBC_PREREQ(2,30)
static pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}
#endif
#endif

class Thread:public noncopyable
{
public:
	typedef std::function<void()> ThreadFunc;

	explicit Thread(const ThreadFunc& func, const std::string& name = std::string()) 
		:m_func(func), 
		m_name(name),
		m_isStarted(false),
		m_tid(0),
		m_isJoined(false)
	{
		CurrentThread::t_name = "main";
		CurrentThread::tid();
		
	}
	~Thread() {}

	//创建线程
	void start();

	//回收线程
	void join();

	//线程是否在运行
	bool isStarted() { return m_isStarted; }

	//线程名
	std::string& name() { return m_name; }
private:

	class ThreadData
	{
	public:
		ThreadData(const ThreadFunc& func, const std::string& name, const std::shared_ptr<pid_t>& tid)
			:m_func(func),
			m_name(name),
			wkTid(tid)
		{}
		
		//在当前线程中运行函数
		void run();

	private:
		ThreadFunc m_func;
		std::string m_name;
		std::weak_ptr<pid_t> wkTid;
	};

private:

	pid_t m_tid;
	ThreadFunc m_func;
	std::string m_name;
	bool m_isStarted;//该线程是否在运行
	bool m_isJoined;//该线程是否已被回收

	static std::atomic<int> g_threadNum;//记录线程数量
};

//CurrentThread类，存储当前线程的信息
class  CurrentThread :public noncopyable
{
	friend class Thread;//提供访问name和tid的权限
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

thread_local std::string CurrentThread::t_name = "unnamedThread";
thread_local pid_t CurrentThread::t_tid = 0;


void Thread::ThreadData::run()
{
	//pid_t tid=
}

std::atomic<int> Thread::g_threadNum = 0;

void Thread::start()
{
	assert(!m_isStarted);
	m_isStarted = true;


}
