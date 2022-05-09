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
#else
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <linux/unistd.h>
#endif



class Thread:public noncopyable
{
public:
	typedef std::function<void()> ThreadFunc;

	explicit Thread(const ThreadFunc& func, const std::string& name = std::string()) 
		:m_func(func), 
		m_name(name),
		m_isStarted(false),
#ifdef __linux__
		m_tid(0),
#endif
		m_isJoined(false)
	{
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
#ifdef _WIN32
	unsigned long gettid()
	{
		return ::GetCurrentThreadId();//获取当前线程id
	}
#endif

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

class  CurrentThread:public noncopyable
{
public:
	static pthread_t tid();//线程ID
	static std::string name();//线程名
	static bool isMainThread();//是否为主线程
private:

}

void Thread::ThreadData::run()
{
	pid_t tid=
}

std::atomic<int> Thread::g_threadNum = 0;

void Thread::start()
{
	assert(!m_isStarted);
	m_isStarted = true;


}
