#pragma once
#include "EventLoop.h"
#include "../logger/logging.h"
#include "../net/Poller.h"
#include "Channel.h"
#include <sys/poll.h>

constexpr int KPollTimeMs = 60 * 1000;

//每个线程只能有一个EventLoop对象
__thread EventLoop* t_loopInThisThread = nullptr;

EventLoop::EventLoop()
	:m_looping(false),
	t_threadId(CurrentThread::tid()),
	m_poller(new Poller(this))
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
	m_quit = false;

	while (!m_quit)
	{
		m_activeChannels.clear();
		m_poller->poll(KPollTimeMs, &m_activeChannels);
		for (auto iter = m_activeChannels.begin(); iter != m_activeChannels.end(); ++iter)
		{
			(*iter)->handleEvent();//处理事件
		}
	}
	LOG_TRACE << "EventLoop " << this << " stop looping";
	m_looping = false;
}

void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	m_poller->updateChannel(channel);
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
	return t_loopInThisThread;
}

void EventLoop::abortNotInLoopThread()
{
	abort();
}
