#include "Connector.h"
#include "../event/EventLoop.h"
#include "Socket.h"
#include "../logger/logging.h"
#include "../event/Channel.h"

#include <iostream>

constexpr int kInitRetryDelayMs = 500;
constexpr int kMaxRetryDelayMs = 30 * 1000;

Connctor::Connctor(EventLoop* loop, const InetAddress& serverAddr)
	:m_loop(loop),
	m_serverAddr(serverAddr),
	m_state(kDisconnected),
	m_connect(false),
	m_retryDelayMs(kInitRetryDelayMs)
{

}

Connctor::~Connctor()
{

}

void Connctor::start()
{
	m_connect = true;
	m_loop->runInLoop(std::bind(&Connctor::startInLoop, this));
}

void Connctor::restart()
{
	m_loop->assertInLoopThread();
	setState(kDisconnected);
	m_retryDelayMs = kInitRetryDelayMs;
	m_connect = true;
	startInLoop();
}

void Connctor::stop()
{
	m_connect = false;
	m_loop->runInLoop(std::bind(&Connctor::stopInLoop, this));
}

void Connctor::startInLoop()
{
	m_loop->assertInLoopThread();
	assert(m_state == kDisconnected);
	if (m_connect)
	{
		connect();
	}
}

void Connctor::stopInLoop()
{
	m_loop->assertInLoopThread();
	if (m_state == kConnecting)
	{
		setState(kDisconnected);
		int sockfd = removeAndResetChannel();
		retry(sockfd);
	}
}

void Connctor::connect()
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

void Connctor::connecting(int sockfd)
{
	setState(kConnecting);
	assert(!m_channel);
	m_channel.reset(new Channel(m_loop, sockfd));
	m_channel->setWriteCallback(std::bind(&Connctor::handleWrite, this));
	m_channel->setErrorCallback(std::bind(&Connctor::handleError, this));
	m_channel->enableWriting();
}

void Connctor::handleWrite()
{
	if (m_state == kConnecting)
	{
		int sockfd = removeAndResetChannel();
		int err = Socket::getSocketError(sockfd);
		if (err)
		{
			LOG_WARN << "Connctor::handleWrite errno=" << err << strerror(err);
			retry(sockfd);
		}
		else if (Socket::isSelfConnect(sockfd))
		{
			LOG_WARN << "Connctor::handleWrite self connect";
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

void Connctor::handleError()
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

void Connctor::retry(int sockfd)
{
	Socket::close(sockfd);
	setState(kDisconnected);
	if (m_connect)
	{
		LOG_INFO << " Connctor::retry " << m_serverAddr.ipString() << ":" << m_serverAddr.port() << " delay:" << m_retryDelayMs;
		//注意当前对象须为由shared_ptr管理的对象
		m_loop->runAfter(m_retryDelayMs / 1000, std::bind(&Connctor::startInLoop, shared_from_this()));//防止等待过程中，当前对象被销毁
		m_retryDelayMs = std::min(m_retryDelayMs * 2, kMaxRetryDelayMs);//每次delay时间翻倍，但不超过30秒
	}
}

int Connctor::removeAndResetChannel()
{
	m_channel->disableALL();
	m_channel->remove();
	int sockfd = m_channel->fd();
	m_loop->queueInLoop(std::bind(&Connctor::resetChannel, this));
	return sockfd;
}

void Connctor::resetChannel()
{
	m_channel.reset();
}

