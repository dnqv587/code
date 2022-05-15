#include "thread.h"

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

std::atomic<int> Thread::g_threadNum;

thread_local std::string CurrentThread::t_name = "unnamedThread";
thread_local pid_t CurrentThread::t_tid = 0;

void afterFork() //fork���̺��ӽ��̵����߳����³�ʼ��
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

ThreadInitializer init;//ʹ��ȫ�ֱ�����ʼ�����߳�

pid_t CurrentThread::tid() //�߳�ID
{
	if (t_tid == 0)
	{
		t_tid = ::gettid();
	}
	return t_tid;
}

std::string CurrentThread::name() //�߳���
{
	return t_name;
}

bool CurrentThread::isMainThread() //�Ƿ�Ϊ���߳�
{
	return t_tid == ::gettid();
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

//�̻߳ص�����
static void* thread(void* arg)
{
	ThreadData* data = static_cast<ThreadData*> (arg);
	data->run();//���к���
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