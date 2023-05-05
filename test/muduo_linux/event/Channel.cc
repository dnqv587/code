#include "Channel.h"
#include "EventLoop.h"
#include "../logger/logging.h"

#include <sys/poll.h>
#include <sys/epoll.h>


#ifdef _USE_EPOLL_
const int Channel::kNoneEvent = NULL;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

constexpr int kReadRevent = EPOLLIN | EPOLLPRI | EPOLLRDHUP;
constexpr int kErrorRevent = EPOLLERR | POLLNVAL;
constexpr int kWriteRevent = EPOLLOUT;
constexpr int kFDNVAL = POLLNVAL;
#else
const int Channel::kNoneEvent = NULL;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

constexpr int kReadRevent = POLLIN | POLLPRI | POLLRDHUP;
constexpr int kErrorRevent = POLLERR | POLLNVAL;
constexpr int kWriteRevent = POLLOUT;
constexpr int kFDNVAL = POLLNVAL;
#endif

static_assert(EPOLLIN == POLLIN);
static_assert(EPOLLPRI == POLLPRI);
static_assert(EPOLLRDHUP == POLLRDHUP);
static_assert(EPOLLERR == POLLERR);
static_assert(EPOLLOUT == POLLOUT);

Channel::Channel(EventLoop* loop, const int fd)
	:m_loop(loop),
	m_fd(fd),
	m_events(NULL),
	m_revents(NULL),
	m_index(-1),
	m_eventHandling(false)
{

}

Channel::~Channel()
{
	//断言当前不在处理事件
	assert(!m_eventHandling);
}

void Channel::handleEvent(Timestamp receiveTime)
{
	m_eventHandling = true;
	if (m_revents & kFDNVAL)
	{
		LOG_WARN << "Channel::handleEvent() POLLNVAL";
	}
	if (m_revents & kErrorRevent)
	{
		if (m_errorCallback)
		{
			m_errorCallback();//错误事件回调
		}
	}
	if (m_revents & kReadRevent)
	{
		if (m_readCallback)
		{
			m_readCallback(receiveTime);//读事件回调
		}
	}
	if (m_revents & kWriteRevent)
	{
		if (m_writeCallback)
		{
			m_writeCallback();//写事件回调
		}
	}
	m_eventHandling = false;
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




