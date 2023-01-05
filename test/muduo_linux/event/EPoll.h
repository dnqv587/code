#pragma once
#include "Poller.h"
#include <vector>


class EventLoop;
struct epoll_event;
class EPoll :public Poller
{
public:
	EPoll(EventLoop* loop);
	~EPoll();

	Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;

	void updateChannel(Channel* channel) override;

	void removeChannel(Channel* channel) override;

private:
	void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

	void update(int operation, Channel* channel);

	using EventList = std::vector<struct epoll_event>;
	int m_epollfd;
	EventList m_events;
};

