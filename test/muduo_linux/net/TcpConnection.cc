#include "TcpConnection.h"
#include "../base/Buffer.h"
#include "../event/Channel.h"
#include "../event/EventLoop.h"
#include "../logger/logging.h"
#include <unistd.h>


TcpConnection::TcpConnection(EventLoop* loop, const std::string& name, int sockfd, const InetAddress& localAddr, const InetAddress& peerAddr)
	:m_loop(loop),
	m_name(name),
	m_state(kConnecting),
	m_socket(std::make_shared<Socket>(sockfd)),
	m_channel(std::make_shared<Channel>(m_loop,sockfd)),
	m_localAddr(localAddr),
	m_peerAddr(peerAddr)
{
	m_channel->setReadCallback(std::bind(&TcpConnection::handleRead, this,std::placeholders::_1));
	//m_channel->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
	m_channel->setErrorCallback(std::bind(&TcpConnection::handleError, this));
	//m_channel->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
}

TcpConnection::~TcpConnection()
{
	LOG_DEBUG << "~TcpConnection [" << m_name << "] at" << this << " fd=" << m_channel->fd() << " state=" << m_state;
	assert(m_state == kDisconnected);
}

void TcpConnection::send(const std::string& msg)
{
	this->send(msg.data(), msg.size());
}

void TcpConnection::send(const void* msg, size_t len)
{
	if (m_state == kConnected)
	{
		if (m_loop->isInLoopThread())
		{
			sendInLoop(msg, len);
		}
		else
		{
			m_loop->runInLoop(std::bind(&TcpConnection::sendInLoop, this, msg, len));
		}
	}
}

void TcpConnection::shutdown()
{
	if (m_state == kConnected)
	{
		setState(kDisconnecting);
		m_loop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
	}
}

void TcpConnection::connectEstablished()
{
	m_loop->assertInLoopThread();
	assert(m_state == kConnecting);
	this->setState(kConnected);
	m_channel->enableReading();
	m_connectionCallback(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
	m_loop->assertInLoopThread();
	assert(m_state == kConnected | m_state == kDisconnecting);
	setState(kDisconnected);
	//主动断开连接，所以再次disableALL
	m_channel->disableALL();
	//通知给用户连接断开
	m_connectionCallback(shared_from_this());

	m_loop->removeChannel(m_channel.get());
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
	m_loop->assertInLoopThread();
	int saveError = NULL;
	ssize_t n = m_inputBuffer.readFd(m_channel->fd(), &saveError);
	switch (n)
	{
	case 0:
		handleClose();
		break;
	case -1:
		errno = saveError;
		handleError();
		break;
	default:
		m_messageCallback(shared_from_this(), &m_inputBuffer, receiveTime);
	}
	
}


void TcpConnection::handleError()
{
	int err = m_socket->getSocketError();
	LOG_ERROR << "TcpConnection::handleError [" << m_name << "] errno=" << err << "  " << ::strerror(err);
}

void TcpConnection::handleClose()
{
	m_loop->assertInLoopThread();
	LOG_TRACE << "TcpConnection::handleClose state=" << (m_state == StateE::kConnected ? "kConnected" : "kConnecting");
	assert(m_state == kConnected| m_state == kDisconnecting);
	m_channel->disableALL();
	m_closeCallback(shared_from_this());
}



void TcpConnection::sendInLoop(const void* data,size_t len)
{
	m_loop->assertInLoopThread();
	ssize_t nwrote = 0;
	if (!m_channel->isWriting() && m_outputBuffer.readableBytes() == 0)
	{
		nwrote = ::write(m_channel->fd(), data, len);
		if (nwrote >= 0)
		{
			if (implicit_cast<size_t>(nwrote) < len)
			{
				LOG_TRACE << "TcpConnection::sendInLoop write:" << nwrote << " all:" << len;
			}
		}
		else
		{
			nwrote = 0;
			if (errno != EWOULDBLOCK)
			{
				LOG_SYSERR << "TcpConnection::sendInLoop error";
			}
		}
		
	}
}

void TcpConnection::shutdownInLoop()
{
	m_loop->assertInLoopThread();
	if (!m_channel->isWriting())
	{
		m_socket->shutdownWrite();
	}
}
