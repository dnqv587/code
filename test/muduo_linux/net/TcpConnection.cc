#include "TcpConnection.h"
#include "../base/Buffer.hpp"
#include "../event/Channel.h"
#include <unistd.h>

void TcpConnection::handleRead()
{
	char buf[65536];
	ssize_t n = ::read(m_channel->fd(), buf, sizeof buf);
	if (n == 0)
	{
		m_socket->close(m_channel->fd());
		return;
	}
	m_messageCallback(shared_from_this(), buf, n);
}
