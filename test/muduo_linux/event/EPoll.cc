#include "EPoll.h"
#include <sys/epoll.h>

EPoll::EPoll(EventLoop* loop)
	:Poller(loop),
	m_epollfd(::epoll_create1(EPOLL_CLOEXEC))
{

}
