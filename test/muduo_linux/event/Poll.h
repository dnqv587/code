#pragma once
#include "../net/Poller.h"
#include <vector>

class EventLoop;
struct epoll_event;
class Poll :public Poller
{
public:
	Poll(EventLoop* loop);
	~Poll();

	Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;

	void updateChannel(Channel* channel) override;

	void removeChannel(Channel* channel) override;

private:
	//遍历m_pollfds，找出活动事件的fd，将对应事件的Channel填入activeChannels
	void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

	typedef std::vector<struct pollfd> PollFdList;

	PollFdList m_pollfds;//fd容器
};