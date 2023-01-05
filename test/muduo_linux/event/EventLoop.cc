#pragma once
#include "EventLoop.h"
#include "../logger/logging.h"
#include "Poll.h"
#include "EPoll.h"
#include "Channel.h"
#include "../time/TimerQueue.h"
#include <sys/poll.h>
#include <unistd.h>
#include <signal.h>
#include <sys/eventfd.h>

constexpr int KPollTimeMs = 60 * 1000;

//每个线程只能有一个EventLoop对象
__thread EventLoop* t_loopInThisThread = nullptr;


//忽略信号SIGPIPE，防止对方断开连接后继续发送数据导致进程退出
class IgnoreSigPipe
{
public:
	IgnoreSigPipe()
	{
		::signal(SIGPIPE, SIG_IGN);
	}
};

IgnoreSigPipe g_ignoreSIGPIPE;

extern "C"
{
	static int createEventfd()
	{
		int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
		if (0 > eventfd)
		{
			LOG_SYSERR << "Failed in eventfd";
			abort();
		}
		return eventfd;
	}
}

EventLoop::EventLoop()
	:m_looping(false),
	m_quit(false),
	m_callPendingFunctor(false),
	t_threadId(CurrentThread::tid()),
	m_timerQueue(new TimerQueue(this)),
	m_poller(new EPoll(this)),
	m_wakeupFd(createEventfd()),
	m_wakeupChannel(new Channel(this, m_wakeupFd))
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
	m_wakeupChannel->setReadCallback(std::bind(&EventLoop::handleRead, this));
	m_wakeupChannel->enableReading();
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
		Timestamp pollReturnTime = m_poller->poll(KPollTimeMs, &m_activeChannels);
		for (auto iter = m_activeChannels.begin(); iter != m_activeChannels.end(); ++iter)
		{
			(*iter)->handleEvent(pollReturnTime);//处理事件
		}
		this->doPendingFunctors();//执行回调定时回调函数
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

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	if (m_eventHandling)
	{
		//
	}
	m_poller->removeChannel(channel);
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
	return t_loopInThisThread;
}

TimerID EventLoop::runAt(const Timestamp time, const TimerCallback& cb)
{
	return m_timerQueue->addTimer(cb, time, 0.0);
}

TimerID EventLoop::runAfter(double delay, const TimerCallback& cb)
{
	return m_timerQueue->addTimer(cb, Timestamp::now() + delay, 0.0);
}

TimerID EventLoop::runEvery(double interval, const TimerCallback& cb)
{
	return m_timerQueue->addTimer(cb, Timestamp::now() + interval, interval);
}

void EventLoop::runInLoop(const Functor& cb)
{
	if (this->isInLoopThread())
	{
		cb();
	}
	else
	{
		this->queueInLoop(cb);
	}
}

void EventLoop::queueInLoop(const Functor& cb)
{
	//因为m_pendingFunctors暴露给了其他线程，所以需要锁
	{
		MutexLockGuard lock(m_mutex);
		m_pendingFunctors.push_back(cb);//添加到pending队列当中
	}
	if (!this->isInLoopThread() || m_callPendingFunctor)
	{
		this->wakeup();//唤醒被阻塞的eventloop线程
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(m_wakeupFd, &one, sizeof one);
	if (n != sizeof one)
	{
		LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}

void EventLoop::abortNotInLoopThread()
{
	abort();
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n;
	do {
		n = ::read(m_wakeupFd, &one, sizeof one);
	} while (errno == EINTR);

	if (n != sizeof one)
	{
		LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	m_callPendingFunctor = true;
	{
		MutexLockGuard lock(m_mutex);
		functors.swap(m_pendingFunctors);//防止调用queueInLoop造成死锁
	}
	for (std::vector<Functor>::iterator iter = functors.begin(); iter != functors.end(); ++iter)
	{
		(*iter)();//执行回调
	}
	m_callPendingFunctor = false;
}
