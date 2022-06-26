#include "thread.h"

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

std::atomic<int> Thread::g_threadNum;

thread_local std::string CurrentThread::t_name = "unnamedThread";
thread_local pid_t CurrentThread::t_tid = 0;

void afterFork() //fork进程后将子进程的主线程重新初始化
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

ThreadInitializer init;//使用全局变量初始化主线程

pid_t CurrentThread::tid() //线程ID
{
	if (t_tid == 0)
	{
		t_tid = ::gettid();
	}
	return t_tid;
}

std::string CurrentThread::name() //线程名
{
	return t_name;
}

bool CurrentThread::isMainThread() //是否为主线程
{
	return t_tid == ::gettid();
}


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


Thread::Thread(const ThreadFunc& func, const std::string& n)
	:m_func(func),
	m_name(n),
	m_isStarted(false),
	m_tid(0),
	m_isJoined(false),
	m_pTid(new pid_t(0))
{
	++g_threadNum;
}

Thread::~Thread()
{
	if (m_isStarted && !m_isJoined)
	{
		pthread_detach(m_tid);
	}
}

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

int Thread::join()
{
	assert(m_isStarted);
	assert(!m_isJoined);
	m_isJoined = true;
	return pthread_join(m_tid, NULL);
}