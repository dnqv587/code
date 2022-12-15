#include "TcpConnection.h"
#include "../base/Buffer.hpp"
#include "../event/Channel.h"
#include "../event/EventLoop.h"
#include <unistd.h>


TcpConnection::TcpConnection(EventLoop* loop, const std::string name, int sockfd, const InetAddress& localAddr, InetAddress& peerAddr)
	:m_loop(loop),
	m_name(name),
	m_state(kConnecting),
	m_socket(std::make_shared<Socket>(sockfd)),
	m_channel(std::make_shared<Channel>(m_loop,sockfd)),
	m_localAddr(localAddr),
	m_peerAddr(peerAddr)
{
	m_channel->setReadCallback(std::bind(&TcpConnection::handleRead, this));
	m_channel->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
	m_channel->setErrorCallback(std::bind(&TcpConnection::handleError, this));
	//m_channel->setCloseCallback();
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::connectEstablished()
{
	m_loop->assertInLoopThread();
	assert(m_state == kConnecting);
	this->setState(kConnected);
	m_channel->enableReading();
	m_connectionCallback(shared_from_this());
}

void TcpConnection::handleRead()
{
	char buf[65536];
	ssize_t n = ::read(m_channel->fd(), buf, sizeof buf);
	if (n == 0)
	{
		TcpConnection::~TcpConnection();
		return;
	}
	m_messageCallback(shared_from_this(), buf, n);
}

void TcpConnection::handleWrite()
{

}

void TcpConnection::handleError()
{
}
