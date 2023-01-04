#pragma once
#include "../net/Poller.h"


class EventLoop;
class EPoll :public Poller
{
public:
	EPoll(EventLoop* loop);
	~EPoll();

	Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;

	void updateChannel(Channel* channel) override;

	void removeChannel(Channel* channel) override;

private:
	int m_epollfd;
};

