#include "Socket.h"
#include "../logger/logging.h"
#include "../base/Exception.h"
#include "InetAddress.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static union
{
	short _s = 0x0102;
	char _cc[sizeof(short)];
};

namespace detail
{
	/// <summary>
	/// 关闭连接
	/// </summary>
	/// <param name="sockfd">文件描述符</param>
	void close(int sockfd)
	{
		if (::close(sockfd) < 0)
		{
			throw close_socket_fail();
			LOG_ERROR << "socket detail::close error ";
		}
	}
	/// <summary>
	/// 创建非阻塞socket
	/// </summary>
	/// <param name="domain">协议族版本</param>
	/// <returns>fd</returns>
	int create_nio_socket(sa_family_t family)
	{
		int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
		if (sockfd < 0)
		{
			throw create_socket_fail();
			LOG_ERROR << "socket detail::create_nio_socket error";
		}
		return sockfd;
	}
	/// <summary>
	/// 创建阻塞socket
	/// </summary>
	/// <param name="family">协议族版本</param>
	/// <returns></returns>
	int create_bio_socket(sa_family_t family)
	{
		int sockfd = ::socket(family, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
		if (sockfd < 0)
		{
			throw create_socket_fail();
			LOG_ERROR << "socket detail::create_bio_socket error";
		}
		return sockfd;
	}

	int accept(int sockfd, struct sockaddr_in6* addr)
	{
		socklen_t addrLen = static_cast<socklen_t>(sizeof * addr);

#if VALGRIND || defined(NO_ACCEPT4)
		int connfd = ::accept(sockfd, Socket::sockaddr_cast(addr), &addrLen);
		Socket::setNonBlockAndCloseOnExec(connfd);
#else//GNU
		int connfd = ::accept4(sockfd, Socket::sockaddr_cast(addr), &addrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
		if (connfd < 0)
		{
			LOG_SYSERR << "accept failed " << errno << ::strerror(errno);
		}
		return connfd;
	}
}


Socket::~Socket()
{
	try
	{
		detail::close(m_sockfd);
	}
	catch (std::exception& ex)
	{
		throw ex;
	}

}

int Socket::create(sa_family_t family, IO type = NIO)
{
	int socket;
	if (BIO == type)
	{
		try
		{
			socket = detail::create_bio_socket(family);
		}
		catch (std::exception& ex)
		{
			socket = -1;
			throw ex;
		}

	}
	else
	{
		try
		{
			socket = detail::create_nio_socket(family);
		}
		catch (std::exception& ex)
		{
			socket = -1;
			throw ex;
		}
	}
	return socket;
}

void Socket::setReuseAddr(bool on)
{
	int val = on ? 1 : 0;
	if (::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof val) && on)
	{
		LOG_SYSERR << "setReuseAddr failed " << ::strerror(errno);
	}
}

void Socket::bindAddress(const InetAddress& addr)
{
	if (::bind(m_sockfd, addr.getSockAddr(), sizeof(sockaddr_in6)))
	{
		LOG_SYSERR << "bindAddress failed " << ::strerror(errno);
	}
}

void Socket::listen()
{
	if (::listen(m_sockfd, SOMAXCONN))
	{
		LOG_SYSERR << "Socket::listen defailed " << ::strerror(errno);
	}
}

int Socket::accept(InetAddress* peerAddr)
{
	struct sockaddr_in6 addr;
	memZero(&addr, sizeof addr);
	int connfd = detail::accept(m_sockfd, &addr);
	if (connfd >= 0)
	{
		peerAddr->setSockAddrInet6(addr);
	}
	return connfd;
}

void Socket::close(int sockfd)
{
	try
	{
		detail::close(sockfd);
	}
	catch (std::exception ex)
	{
		throw ex;
	}
	
}

void Socket::fromIpPort(std::string& ip, in_port_t port, sockaddr_in* addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = Endian::HostToNetwork16(port);
	if (-1 == ::inet_pton(AF_INET, ip.c_str(), &addr->sin_addr.s_addr))
	{
		throw pton_error();
		LOG_SYSERR << ::strerror(errno);
	}
}

void Socket::fromIpPort(std::string& ip, in_port_t port, sockaddr_in6* addr)
{
	addr->sin6_family = AF_INET6;
	addr->sin6_port = Endian::HostToNetwork16(port);
	if (-1 == ::inet_pton(AF_INET6, ip.c_str(), &addr->sin6_addr))
	{
		throw pton_error();
		LOG_SYSERR << ::strerror(errno);
	}
}

std::string Socket::toIpString(const struct sockaddr* addr)
{
	char ip[64];
	if (addr->sa_family == AF_INET)
	{
		uint32_t src = Endian::NetworkToHost32(sockaddr_in_cast(addr)->sin_addr.s_addr);
		if (!::inet_ntop(AF_INET, &src, ip, sizeof ip / sizeof ip[0]))
		{
			throw ntop_error();
			LOG_SYSERR << ::strerror(errno);
		}
	}
	else if (addr->sa_family == AF_INET6)
	{
		if (!::inet_ntop(AF_INET6, &sockaddr_in6_cast(addr)->sin6_addr, ip, sizeof ip / sizeof ip[0]))
		{
			throw ntop_error();
			LOG_SYSERR << ::strerror(errno);
		}
	}
	return ip;
}


void Socket::setNonBlockAndCloseOnExec(int sockfd)
{
	// non-block
	int flags = ::fcntl(sockfd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	int ret = ::fcntl(sockfd, F_SETFL, flags);
	// FIXME check

	// close-on-exec
	flags = ::fcntl(sockfd, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	ret = ::fcntl(sockfd, F_SETFD, flags);
	// FIXME check

	(void)ret;
}

bool Endian::isLittleEndian()
{
	return (_cc[0] == 0x02 && _cc[1] == 0x01);
}

bool Endian::isBigEndian()
{
	return (_cc[0] == 0x01 && _cc[1] == 0x02);
}

