#include "Channel.h"
#include "EventLoop.h"
#include "../logger/logging.h"

#include <sys/poll.h>


const int Channel::kNoneEvent = NULL;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, const int fd)
	:m_loop(loop),
	m_fd(fd),
	m_events(NULL),
	m_revents(NULL),
	m_index(-1)
{

}

void Channel::handleEvent()
{
	if (m_revents & POLLNVAL)
	{
		LOG_WARN << "Channel::handleEvent() POLLNVAL";
	}
	if (m_revents & (POLLERR | POLLNVAL))
	{
		if (m_errorCallback)
		{
			m_errorCallback();//错误事件回调
		}
	}
	if (m_revents & (POLLIN | POLLPRI | POLLRDHUP))
	{
		if (m_readCallback)
		{
			m_readCallback();//读事件回调
		}
	}
	if (m_revents & POLLOUT)
	{
		if (m_writeCallback)
		{
			m_writeCallback();//写事件回调
		}
	}

}

void Channel::remove()
{
	assert(isNoneEvent());

	m_loop->removeChannel(this);
}

void Channel::update()
{
	m_loop->updateChannel(this);
}




