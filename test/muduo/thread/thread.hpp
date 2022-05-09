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

	//�����߳�
	void start();

	//�����߳�
	void join();

	//�߳��Ƿ�������
	bool isStarted() { return m_isStarted; }

	//�߳���
	std::string& name() { return m_name; }
private:
#ifdef _WIN32
	unsigned long gettid()
	{
		return ::GetCurrentThreadId();//��ȡ��ǰ�߳�id
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
		
		//�ڵ�ǰ�߳������к���
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
	bool m_isStarted;//���߳��Ƿ�������
	bool m_isJoined;//���߳��Ƿ��ѱ�����

	static std::atomic<int> g_threadNum;//��¼�߳�����
};

class  CurrentThread:public noncopyable
{
public:
	static pthread_t tid();//�߳�ID
	static std::string name();//�߳���
	static bool isMainThread();//�Ƿ�Ϊ���߳�
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
