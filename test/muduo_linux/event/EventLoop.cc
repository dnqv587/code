#pragma once
#include "EventLoop.h"
#include "../logger/logging.h"
#include <sys/poll.h>

__thread EventLoop* t_loopInThisThread = nullptr;

EventLoop::EventLoop()
	:m_looping(false),
	t_threadId(CurrentThread::tid())
{
	if (t_loopInThisThread)
	{
		//当前线程存在EventLoop
		LOG_FATAL << "Another EventLoop" << t_loopInThisThread << "exists in this thread" << t_threadId;
	}
	else
	{
		t_loopInThisThread = this;
	}
}

EventLoop::~EventLoop()
{
	t_loopInThisThread = nullptr;
}

void EventLoop::loop()
{
	assert(!m_looping);
	assertInLoopThread();
	m_looping = true;

	::poll(NULL, 0, 5 * 1000);

	LOG_TRACE << "EventLoop " << this << " stop looping";
	m_looping = false;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
	return t_loopInThisThread;
}

void EventLoop::abortNotInLoopThread()
{
	abort();
}
