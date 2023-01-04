#include "Connector.h"
#include "../event/EventLoop.h"
#include "Socket.h"
#include "../logger/logging.h"
#include "../event/Channel.h"

#include <iostream>

constexpr int kInitRetryDelayMs = 500;
constexpr int kMaxRetryDelayMs = 30 * 1000;

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
	:m_loop(loop),
	m_serverAddr(serverAddr),
	m_state(kDisconnected),
	m_connect(false),
	m_retryDelayMs(kInitRetryDelayMs)
{

}

Connector::~Connector()
{

}

void Connector::start()
{
	m_connect = true;
	m_loop->runInLoop(std::bind(&Connector::startInLoop, this));
}

void Connector::restart()
{
	m_loop->assertInLoopThread();
	setState(kDisconnected);
	m_retryDelayMs = kInitRetryDelayMs;
	m_connect = true;
	startInLoop();
}

void Connector::stop()
{
	m_connect = false;
	m_loop->runInLoop(std::bind(&Connector::stopInLoop, this));
}

void Connector::startInLoop()
{
	m_loop->assertInLoopThread();
	assert(m_state == kDisconnected);
	if (m_connect)
	{
		connect();
	}
}

void Connector::stopInLoop()
{
	m_loop->assertInLoopThread();
	if (m_state == kConnecting)
	{
		setState(kDisconnected);
		int sockfd = removeAndResetChannel();
		retry(sockfd);
	}
}

void Connector::connect()
{
	int sockfd = Socket::create(m_serverAddr.family(), Socket::IO::NIO);
	int ret = Socket::connect(sockfd, m_serverAddr.getSockAddr());
	int saveErrno = (ret == 0) ? 0 : errno;
	switch (saveErrno)
	{
	case 0:
	case EINPROGRESS:
	case EINTR:
	case EISCONN:
		connecting(sockfd);
		break;

	case EAGAIN:
	case EADDRINUSE:
	case EADDRNOTAVAIL:
	case ECONNREFUSED:
	case ENETUNREACH:
		retry(sockfd);
		break;

	case EACCES:
	case EPERM:
	case EAFNOSUPPORT:
	case EALREADY:
	case EBADF:
	case EFAULT:
	case ENOTSOCK:
		LOG_SYSERR << "connect error in Connector::startInLoop " << saveErrno;
		Socket::close(sockfd);
		break;

	default:
		LOG_SYSERR << "Unexpected error in Connector::startInLoop " << saveErrno;
		Socket::close(sockfd);
		break;

	}
}

void Connector::connecting(int sockfd)
{
	setState(kConnecting);
	assert(!m_channel);
	m_channel.reset(new Channel(m_loop, sockfd));
	m_channel->setWriteCallback(std::bind(&Connector::handleWrite, this));
	m_channel->setErrorCallback(std::bind(&Connector::handleError, this));
	m_channel->enableWriting();
}

void Connector::handleWrite()
{
	if (m_state == kConnecting)
	{
		int sockfd = removeAndResetChannel();
		int err = Socket::getSocketError(sockfd);
		if (err)
		{
			LOG_WARN << "Connector::handleWrite errno=" << err << strerror(err);
			retry(sockfd);
		}
		else if (Socket::isSelfConnect(sockfd))
		{
			LOG_WARN << "Connector::handleWrite self connect";
			retry(sockfd);
		}
		else
		{
			setState(KConnected);
			if (m_connect)
			{
				m_newConnectionCallback(sockfd);
			}
			else
			{
				Socket::close(sockfd);
			}
		}
	}
	else
	{
		assert(m_state == kDisconnected);
	}
}

void Connector::handleError()
{
	LOG_ERROR << "Connector::handleError state=" << m_state;
	if (m_state == kConnecting)
	{
		int sockfd = removeAndResetChannel();
		int err = Socket::getSocketError(sockfd);
		LOG_TRACE << "SO_ERROR = " << err << " " << strerror(err);
		retry(sockfd);
	}
}

void Connector::retry(int sockfd)
{
	Socket::close(sockfd);
	setState(kDisconnected);
	if (m_connect)
	{
		LOG_INFO << " Connector::retry " << m_serverAddr.ipString() << ":" << m_serverAddr.port() << " delay:" << m_retryDelayMs;
		//注意当前对象须为由shared_ptr管理的对象
		m_loop->runAfter(m_retryDelayMs / 1000, std::bind(&Connector::startInLoop, shared_from_this()));//防止等待过程中，当前对象被销毁
		m_retryDelayMs = std::min(m_retryDelayMs * 2, kMaxRetryDelayMs);//每次delay时间翻倍，但不超过30秒
	}
}

int Connector::removeAndResetChannel()
{
	m_channel->disableALL();
	m_channel->remove();
	int sockfd = m_channel->fd();
	m_loop->queueInLoop(std::bind(&Connector::resetChannel, this));
	return sockfd;
}

void Connector::resetChannel()
{
	m_channel.reset();
}

