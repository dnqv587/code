#include "Socket.h"
#include "../logger/logging.h"
#include "../base/Exception.h"
#include "InetAddress.h"
#include "../base/Tools.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <netinet/tcp.h>


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
		int connfd;
#if VALGRIND || defined(NO_ACCEPT4)
		do
		{
			connfd = ::accept(sockfd, Socket::sockaddr_cast(addr), &addrLen);
		} while (errno == EINTR);

		Socket::setNonBlockAndCloseOnExec(connfd);
#else//GNU
		do 
		{
			connfd = ::accept4(sockfd, Socket::sockaddr_cast(addr), &addrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
		} while (errno == EINTR);
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

void Socket::close()
{
	try
	{
		detail::close(m_sockfd);
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}


int Socket::connect(int sockfd, const struct sockaddr* addr)
{
	return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(sockaddr_in6)));
}

int Socket::connect(const struct sockaddr* addr)
{
	connect(m_sockfd, addr);
}

bool Socket::isSelfConnect(int sockfd)
{
	struct sockaddr_in6 localaddr = getLocalAddr(sockfd);
	struct sockaddr_in6 peeraddr = getPeerAddr(sockfd);
	if (localaddr.sin6_family == AF_INET)
	{
		const struct sockaddr_in* laddr4 = reinterpret_cast<struct sockaddr_in*>(&localaddr);
		const struct sockaddr_in* raddr4 = reinterpret_cast<struct sockaddr_in*>(&peeraddr);
		return laddr4->sin_port == raddr4->sin_port
			&& laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
	}
	else if (localaddr.sin6_family == AF_INET6)
	{
		return localaddr.sin6_port == peeraddr.sin6_port
			&& memcmp(&localaddr.sin6_addr, &peeraddr.sin6_addr, sizeof localaddr.sin6_addr) == 0;
	}
	else
	{
		return false;
	}
}

bool Socket::isSelfConnect()
{
	isSelfConnect(m_sockfd);
}

void Socket::shutdownWrite()
{
	if (::shutdown(m_sockfd, SHUT_WR) == -1)
	{
		LOG_SYSERR << "Socket::shutdownWrite error";
	}
}

void Socket::shutdownWrite(int sockfd)
{
	if (::shutdown(sockfd, SHUT_WR) == -1)
	{
		LOG_SYSERR << "Socket::shutdownWrite error";
	}
}

void Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	if (::setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof(optval))) == -1)
	{
		LOG_SYSERR << "Socket::setTcpNoDelay";
	}
}

void Socket::setKeepAlive(bool on)
{
	int optval = on ? 1 : 0;
	if (::setsockopt(m_sockfd, IPPROTO_TCP, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof(optval))) == -1)
	{
		LOG_SYSERR << "Socket::setKeepAlive";
	}
}

int Socket::getSocketError()
{
	int optval;
	socklen_t optlen = static_cast<socklen_t>(sizeof optval);
	if (::getsockopt(m_sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) == -1)
	{
		return errno;
	}
	else
	{
		return optval;
	}
}

int Socket::getSocketError(int sockfd)
{
	int optval;
	socklen_t optlen = static_cast<socklen_t>(sizeof optval);
	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) == -1)
	{
		return errno;
	}
	else
	{
		return optval;
	}
}

ssize_t Socket::read(void* buf, size_t count)
{
	return ::read(m_sockfd, buf, count);
}

ssize_t Socket::read(int sockfd, void* buf, size_t count)
{
	return ::read(sockfd, buf, count);
}

ssize_t Socket::readv(const struct iovec* iov, int iovcnt)
{
	return ::readv(m_sockfd, iov, iovcnt);
}

ssize_t Socket::readv(int sockfd, const struct iovec* iov, int iovcnt)
{
	return ::readv(sockfd, iov, iovcnt);
}

ssize_t Socket::write(const void* buf, size_t count)
{
	return ::write(m_sockfd, buf, count);
}

ssize_t Socket::write(int sockfd,const void* buf, size_t count)
{
	return ::write(sockfd, buf, count);
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
	std::string IP;
	std::vector<std::string> vec = Tools::String::Split(ip, '.');
	for (int n = vec.size()-1; n > 0; --n)
	{
		IP.append(vec[n] + '.');
	}
	IP.append(vec[0] );
	return IP;
}

struct sockaddr_in6 Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in6 localAddr;
	socklen_t len = static_cast<socklen_t>(sizeof(localAddr));
	if (::getsockname(sockfd, sockaddr_cast(&localAddr), &len) == -1)
	{
		LOG_SYSERR << "Socket::getLocalAddr";
	}
	return localAddr;
}

sockaddr_in6 Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in6 peerAddr;
	socklen_t len = static_cast<socklen_t>(sizeof(peerAddr));
	if (::getpeername(sockfd, sockaddr_cast(&peerAddr), &len) == -1)
	{
		LOG_SYSERR << "Socket::getPeerAddr";
	}
	return peerAddr;
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

