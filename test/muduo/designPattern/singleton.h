#pragma once
#include "../base/noncopyable.h"
#include <pthread.h>
#include <assert.h>


/*
����ģʽ
*/
template<class T>
class Singleton :public noncopyable
{
public:
	
	Singleton() = delete;
	~Singleton() = delete;
	
	static T* instance()
	{
		pthread_once(&m_once, init);//ִֻ��һ��ʵ����
		return g_instance;
	}

private:
	static void init()
	{
		g_instance = new T;
		::atexit(destroy);//ע���˳�����
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
