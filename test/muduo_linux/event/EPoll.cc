#include "EPoll.h"
#include "../logger/logging.h"
#include "Channel.h"
#include <sys/epoll.h>
#include <unistd.h>

constexpr int KInitEventListSize = 16;

EPoll::EPoll(EventLoop* loop)
	:Poller(loop),
	m_epollfd(::epoll_create1(EPOLL_CLOEXEC)),
	m_events(KInitEventListSize)
{
	if (m_epollfd < 0)
	{
		LOG_SYSERR << "epoll_create1 error";
	}
}

EPoll::~EPoll()
{
	::close(m_epollfd);
}

Timestamp EPoll::poll(int timeoutMs, ChannelList* activeChannels)
{
	int numEvents = ::epoll_wait(m_epollfd, m_events.data(), static_cast<int>(m_events.size()), timeoutMs);
	int saveErrno = errno;
	Timestamp now(Timestamp::now());
	if (numEvents > 0)
	{
		LOG_TRACE << numEvents << " events happended";
		fillActiveChannels(numEvents, activeChannels);
		//若传出事件数组不足，则扩容为原来的两倍
		if (implicit_cast<size_t>(numEvents) == m_events.size())
		{
			m_events.resize(m_events.size() * 2);
		}
	}
	else if (numEvents == 0)
	{
		LOG_TRACE << "nothing happended";
	}
	else
	{
		LOG_SYSERR << "EPoll::poll epoll_wait error, errno=" << saveErrno << "#" << ::strerror(saveErrno);
	}
	return now;
}

void EPoll::updateChannel(Channel* channel)
{
	Poller::assertInLoopThread();
	if (channel->index() < 0)
	{
		if (channel->index() == -1)
		{
			assert(m_channels.find(channel->fd()) == m_channels.end());
			m_channels.insert(std::make_pair(channel->fd(), channel));
			assert(m_channels[channel->fd()] == channel);
			channel->set_index(m_channels.size() - 1);
		}
		else
		{
			assert(m_channels.find(channel->fd()) != m_channels.end());
			assert(m_channels[channel->fd()] == channel);
			channel->set_index(-channel->index() + 2);
		}
		update(EPOLL_CTL_ADD, channel);
	}
	else
	{
		assert(m_channels.find(channel->fd()) != m_channels.end());
		assert(m_channels[channel->fd()] == channel);
		if (channel->isNoneEvent())
		{
			channel->set_index(-channel->index() - 2);
			update(EPOLL_CTL_DEL, channel);
		}
		else
		{
			update(EPOLL_CTL_MOD, channel);
		}
	}
}

void EPoll::removeChannel(Channel* channel)
{
	Poller::assertInLoopThread();
	LOG_TRACE << "EPoll::removeChannel fd=" << channel->fd();
	assert(m_channels.find(channel->fd()) != m_channels.end());
	assert(m_channels[channel->fd()] == channel);
	assert(channel->isNoneEvent());
	size_t n = m_channels.erase(channel->fd());
	assert(n == 1);
	if (channel->index() >= 0)
	{
		update(EPOLL_CTL_DEL, channel);
	}
	channel->set_index(-channel->index() - 2);
	
}

void EPoll::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
	assert(implicit_cast<size_t>(numEvents) <= m_events.size());
	for (int i = 0; i < numEvents; ++i)
	{
		Channel* channel = static_cast<Channel*>(m_events[i].data.ptr);
#ifndef NDEBUG
		const auto iter = m_channels.find(channel->fd());
		assert(iter != m_channels.cend());
		assert(iter->second == channel);
#endif
		channel->set_revents(m_events[i].events);
		activeChannels->push_back(channel);
	}
}

void EPoll::update(int operation, Channel* channel)
{
	struct epoll_event event;
	memZero(&event, sizeof event);
	event.events = channel->events();
	event.data.ptr = channel;
	LOG_TRACE << "epoll_ctl op = " << " fd = " << channel->fd() << " event = { " << channel->events() << " }";

	if (::epoll_ctl(m_epollfd, operation, channel->fd(), &event) < 0)
	{
		if (operation == EPOLL_CTL_DEL)
		{
			LOG_SYSERR << "epoll_ctl error "  << " fd =" << channel->fd();
		}
		else
		{
			LOG_SYSFATAL << "epoll_ctl error " << " fd =" << channel->fd();
		}
	}
	
}
