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
	return ::GetCurrentThreadId();//��ȡ��ǰ�߳�id
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

static 	void afterFork()//fork���̺��ӽ��̵����߳����³�ʼ��
{
	CurrentThread::t_tid = 0;
	CurrentThread::t_name = "main";
	CurrentThread::tid();
}

class ThreadInitializer //��ʼ���࣬�����������߳������߳�id
{
public:
	ThreadInitializer()
	{
		CurrentThread::t_name = "main";
		CurrentThread::tid();
		pthread_atfork(NULL, NULL, afterFork);//ע��fork�ص�����
	}
};

//�̻߳ص�����
static void* thread(void* arg)
{
	ThreadData* data = static_cast<ThreadData*>(arg);
	data->run();//���к���
	delete data;
	return;
}

//�̺߳���,�߳������еĺ���
class ThreadData
{
	typedef std::function<void()> ThreadFunc;
public:
	ThreadData(const ThreadFunc& func, const std::string& name, const std::shared_ptr<pid_t>& tid)
		:m_func(func),
		m_name(name),
		wkTid(tid)
	{}

	//�ڵ�ǰ�߳������к���
	void run()
	{
		pid_t tid = CurrentThread::tid();
		std::shared_ptr<pid_t> pTid = wkTid.lock();//�ж��߳��Ƿ�����
		if (pTid)
		{
			*pTid = tid;
			pTid.reset();//?
		}

		CurrentThread::t_name = m_name.empty() ? "Thread" : m_name;
		::prctl(PR_SET_NAME, CurrentThread::t_name);//�����߳���

		m_func();//���лص�����
		CurrentThread::t_name = "finished";//�߳̽���

	}

private:
	ThreadFunc m_func;
	std::string m_name;
	std::weak_ptr<pid_t> wkTid;//ʹ��weak_ptr�Ƿ�ֹ��ʹ��ǰ�������߳�������
};



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

	//�����߳�
	void start();

	//�����߳�
	void join();

	//�߳��Ƿ�������
	bool isStarted() { return m_isStarted; }

	//�߳���
	std::string& name() { return m_name; }

private:

	pthread_t m_tid;//�߳�id
	std::shared_ptr<pid_t> m_pTid;//�̱߳�ʶ���ж��߳��Ƿ�����
	ThreadFunc m_func;
	std::string m_name;
	bool m_isStarted;//���߳��Ƿ�������
	bool m_isJoined;//���߳��Ƿ��ѱ�����

	static std::atomic<int> g_threadNum;//��¼�߳�����
};

//CurrentThread�࣬�洢��ǰ�̵߳���Ϣ
class  CurrentThread :public noncopyable
{
	friend class ThreadData;
	friend class ThreadInitializer;//�ṩ����name��tid��Ȩ��
	friend void afterFork();//����tid
public:
	static pid_t tid()//�߳�ID
	{
		if (t_tid == 0)
		{
			t_tid = ::gettid();
		}
		return t_tid;
	}
	static std::string name()//�߳���
	{
		return t_name;
	}
	static bool isMainThread()//�Ƿ�Ϊ���߳�
	{
		return t_tid == ::gettid();
	}
private:
	thread_local static std::string t_name;//��ǰ�߳���
	thread_local static pid_t t_tid;//��ǰ�߳�ID
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
		abort();//�˳�����
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

ThreadInitializer init;//ʹ��ȫ�ֱ�����ʼ�����߳�

std::atomic<int> Thread::g_threadNum;