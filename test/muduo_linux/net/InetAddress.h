#pragma once
#include "../base/copyable.h"
#include "../base/Type.h"
#include "Socket.h"
#include <netinet/in.h>
#include <string>

/// <summary>
/// 网络地址封装类
/// </summary>
class InetAddress :copyable
{
public:
	explicit InetAddress(in_port_t port, bool loopbackOnly = false, bool ipv6 = false);

	InetAddress(std::string addr, in_port_t port, bool ipv6 = false);

	explicit InetAddress(sockaddr_in& addr)
		:_addr(addr)
	{
	}

	explicit InetAddress(sockaddr_in6& addr6)
		:_addr6(addr6)
	{
	}

	const sockaddr* getSockAddr() const
	{
		return Socket::sockaddr_cast(&_addr6);
	}
	/// <summary>
	/// 获取点分十进制IP
	/// </summary>
	/// <returns></returns>
	std::string ipString() const
	{
		return Socket::toIpString(Socket::sockaddr_cast(&_addr6));
	}
	/// <summary>
	/// 获取端口号
	/// </summary>
	/// <returns></returns>
	in_port_t port() const
	{
		return Endian::NetworkToHost16(_addr.sin_port);
	}
	/// <summary>
	/// 获取协议簇
	/// </summary>
	/// <returns></returns>
	sa_family_t family() const
	{
		return _addr.sin_family;
	}
private:

	union
	{
		sockaddr_in _addr;
		sockaddr_in6 _addr6;
	};
};