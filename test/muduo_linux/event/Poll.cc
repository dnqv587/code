#include "Poll.h"
#include "../logger/logging.h"
#include "Channel.h"
#include <sys/poll.h>

Poll::Poll(EventLoop* loop)
	:Poller(loop)
{

}

Poll::~Poll()
{
}

Timestamp Poll::poll(int timeoutMs, ChannelList* activeChannels)
{
	int numEvents = ::poll(m_pollfds.data(), m_pollfds.size(), timeoutMs);//进行监听

	if (numEvents > 0)
	{
		LOG_TRACE << numEvents << " events happended";

		fillActiveChannels(numEvents, activeChannels);//查找活跃的fd
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

void Poll::updateChannel(Channel* channel)
{
	assertInLoopThread();
	LOG_TRACE << "fd= " << channel->fd() << " events= " << channel->events();

	if (channel->index() < 0)//Channel不在m_channels中
	{
		//m_pollfds添加新Channel
		assert(m_channels.find(channel->fd()) == m_channels.end());//断言channel不在m_channels中
		struct pollfd pfd;
		pfd.fd = channel->fd();
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = NULL;
		m_pollfds.push_back(pfd);
		int idx = static_cast<int>(m_pollfds.size()) - 1;
		channel->set_index(idx);
		//m_channels[pfd.fd] = channel;
		m_channels.insert(std::pair<int, Channel*>(pfd.fd, channel));
	}
	else
	{
		//更新
		assert(m_channels.find(channel->fd()) != m_channels.end());
		assert(m_channels.find(channel->fd())->second == channel);
		int idx = channel->index();
		assert(0 <= idx && idx < static_cast<int>(m_pollfds.size()));
		struct pollfd& pfd = m_pollfds.at(idx);//引用m_pollfds对应下标的pollfd进行修改
		assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() - 1);
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = NULL;
		if (channel->isNoneEvent())//监听事件为空
		{
			//忽略此pollfd
			pfd.fd = -channel->fd() - 1;
		}
	}
}

void Poll::removeChannel(Channel* channel)
{
	assertInLoopThread();
	LOG_TRACE << "fd= " << channel->fd();
	assert(m_channels.find(channel->fd()) != m_channels.end());
	assert(m_channels.at(channel->fd()) == channel);
	assert(channel->isNoneEvent());
	int index = channel->index();
	assert(index >= 0 && index < static_cast<int>(m_channels.size()));
	size_t n = m_channels.erase(channel->fd());
	assert(n == 1);
	//从fd容器m_pollfds去除
	if (implicit_cast<size_t>(index) == m_pollfds.size() - 1)
	{
		m_pollfds.pop_back();
	}
	else
	{
		int pollfdEnd = m_pollfds.back().fd;
		std::iter_swap(m_pollfds.begin() + index, m_pollfds.end() - 1);//调换位置
		if (pollfdEnd < 0)
		{
			pollfdEnd = -pollfdEnd - 1;
		}
		m_channels[pollfdEnd]->set_index(index);//调换位置后将index更换
		m_pollfds.pop_back();
	}
}

void Poll::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
	for (auto iter = m_pollfds.cbegin(); iter != m_pollfds.cend() && numEvents; ++iter)
	{
		if (iter->revents > 0)//有事件发生
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
