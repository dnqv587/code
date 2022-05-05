#pragma once
#include "../base/noncopyable.h"
#include <pthread.h>
#include <assert.h>


/*
单例模式
*/
template<class T>
class Singleton :public noncopyable
{
public:
	
	Singleton() = delete;
	~Singleton() = delete;
	
	static T& instance()
	{
		pthread_once(&m_once, init);//只执行一次实例化
		return *g_instance;
	}

private:
	static void init()
	{
		g_instance = new T;
		::atexit(destroy);//注册退出函数
	}
	static void destroy()
	{
		delete g_instance;
	}

	static T* g_instance;
	static pthread_once_t m_once;
};

template<class T>
T* Singleton<T>::g_instance = nullptr;
template<class T>
pthread_once_t Singleton<T>::m_once = PTHREAD_ONCE_INIT;
