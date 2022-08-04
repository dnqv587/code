#include "Poller.h"
#include "../logger/logging.h"
#include "../event/Channel.h"

#include <sys/poll>

Poller::Poller(EventLoop* loop)
	:m_ownerLoop(loop)
{

}

Poller::~Poller()
{

}

Timestamp Poller::poll(int timeoutMs, ChannelList* activeChannels)
{
	int numEvents = ::poll(m_pollfds.data(), m_pollfds.size(), timeoutMs);

	if (numEvents > 0)
	{
		LOG_TRACE << numEvents << " events happended";

		fillActiveChannels(numEvents, activeChannels);
	}
	else if (numEvents == 0)
	{
		LOG_TRACE << "nothing happended";
	}
	else
	{
		LOG_SYSERR << "Poller::poll()";
	}

	return Timestamp::now();
}

void Poller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
	for (auto iter = m_pollfds.cbegin(); iter != m_pollfds.cend()&& numEvents; ++iter)
	{
		if (iter->revents > 0)
		{
			--numEvents;//每次定位到事件fd，则减少numEvents，当为0时不继续遍历
			auto channelMap = m_channels.find(iter->fd);//查找到对应fd的Channel对象map
			assert(channelMap != m_channels.cend());
			Channel* channel = channelMap->second;//Channel对象
			assert(channel->fd() == iter->fd);
			channel->set_revents(iter->revents);//将返回的事件写入到Channel
			activeChannels->push_back(channel);//活跃的Channel写入到activeChannels
		}
	}
}

void Poller::updateChannel(Channel* channel)
{

}
