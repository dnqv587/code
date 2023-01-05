#include "Poller.h"
#include "../logger/logging.h"
#include "../event/Channel.h"

#include <poll.h>

Poller::Poller(EventLoop* loop)
	:m_ownerLoop(loop)
{

}

Poller::~Poller()
{

}


