#pragma once
#include "../base/noncopyable.h"
#include <pthread.h>
#include <atomic>
#include <functional>
#include <memory>
#include <string>



class Thread:private noncopyable
{
public:
	typedef std::function<void()> ThreadFunc;

	Thread(const ThreadFunc& func, const std::string& n = std::string());

	~Thread();

	//创建线程
	void start();

	//回收线程
	int join();

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

class ThreadData;
class ThreadInitializer;
//CurrentThread类，存储当前线程的信息
class  CurrentThread :private noncopyable
{
	friend class ThreadData;
	friend class ThreadInitializer;//提供访问name和tid的权限
	friend void afterFork();//访问tid
public:
	static pid_t tid();//线程ID

	static std::string name();//线程名

	static bool isMainThread();//是否为主线程
private:
	thread_local static std::string t_name;//当前线程名
	thread_local static pid_t t_tid;//当前线程ID
};

