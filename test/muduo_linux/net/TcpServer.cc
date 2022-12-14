#include "TcpServer.h"
#include "../event/EventLoop.h"
#include "../logger/logging.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "functional"


TcpServer::TcpServer(EventLoop* loop, std::string name, const InetAddress& listenAddr)
	:m_loop(loop),
	m_name(name),
	m_acceptor(new Acceptor(m_loop,listenAddr)),
	m_nextConnId(1)
{
	//设置accept的回调，并先占位
	m_acceptor->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

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
	//...
}

