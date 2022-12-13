#include "TcpServer.h"
#include "../event/EventLoop.h"
#include "../logger/logging.h"
#include "InetAddress.h"

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
	m_loop->assertInLoopThread();
	char buf[32];
	snprintf(buf, sizeof buf, "#%d", m_nextConnId);
	++m_nextConnId;
	std::string connName = m_name + buf;
	
	LOG_INFO << "TcpServer::newConnection [" << m_name
		<< "] - new connection [" << connName
		<< "] from " << peerAddr.port();

}

