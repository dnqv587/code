#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include <assert.h>


EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const std::string& name)
	:m_baseLoop(baseLoop),
	m_name(name),
	m_started(false),
	m_numThreads(0),
	m_next(0)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{
}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
	assert(!m_started);
	m_baseLoop->assertInLoopThread();
	m_started = true;

	for (int i = 0; i < m_numThreads; ++i)
	{
		char buf[m_name.size() + 32];
		snprintf(buf, sizeof(buf), "%s%d", m_name.c_str(), i);
		EventLoopThread* t = new EventLoopThread(cb, buf);
		m_threads.push_back(std::unique_ptr<EventLoopThread>(t));
		m_loops.push_back(t->startLoop());
	}
	if (m_numThreads == 0 && cb)
	{
		cb(m_baseLoop);
	}
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
	m_baseLoop->assertInLoopThread();
	assert(m_started);
	EventLoop* loop = m_baseLoop;
	if (!m_loops.empty())
	{
		loop = m_loops[m_next];
		++m_next;
		if (implicit_cast<size_t>(m_next) >= m_loops.size())
		{
			m_next = 0;
		}
	}
	return loop;
}
