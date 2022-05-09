#pragma once
#include "../base/noncopyable.h"
#include <pthread.h>
#include <atomic>
#include <functional>
#include <memory>



class thread:public noncopyable
{
public:
	

private:
	pthread_t m_self;

};