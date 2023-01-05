#include "Poller.h"
#include "../logger/logging.h"
#include "../event/Channel.h"
#include "Poll.h"
#include "EPoll.h"
#include <poll.h>

Poller::Poller(EventLoop* loop)
	:m_ownerLoop(loop)
{

}

Poller::~Poller()
{

}

Poller* Poller::newPoller(EventLoop* loop)
{
	if (::getenv("_USE_EPOLL_"))
	{
		return new EPoll(loop);
	}
	else
	{
		return new Poll(loop);
	}
}


