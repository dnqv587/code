#include "Socket.h"
#include "../logger/logging.h"
#include "../base/Exception.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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

void Socket::fromIpPort(std::string ip, in_port_t port, sockaddr_in* addr)
{

}
