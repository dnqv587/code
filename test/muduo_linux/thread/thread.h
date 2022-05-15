#pragma once
#include "../base/noncopyable.h"
#include <pthread.h>
#include <atomic>
#include <functional>
#include <memory>
#include <string>



class Thread:public noncopyable
{
public:
	typedef std::function<void ()> ThreadFunc;

	Thread(const ThreadFunc& func, const std::string& n = std::string());

	~Thread();

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

class ThreadData;
class ThreadInitializer;
//CurrentThread�࣬�洢��ǰ�̵߳���Ϣ
class  CurrentThread :public noncopyable
{
	friend class ThreadData;
	friend class ThreadInitializer;//�ṩ����name��tid��Ȩ��
	friend void afterFork();//����tid
public:
	static pid_t tid();//�߳�ID

	static std::string name();//�߳���

	static bool isMainThread();//�Ƿ�Ϊ���߳�
private:
	thread_local static std::string t_name;//��ǰ�߳���
	thread_local static pid_t t_tid;//��ǰ�߳�ID
};

