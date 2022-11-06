#pragma once
#include "../base/noncopyable.h"
#include <bits/sockaddr.h>
#include <endian.h>
#include <stdint.h>
#include <string>
#include <netinet/in.h>

/// <summary>
/// 封装socket文件描述符
/// 符合RAII
/// </summary>
class Socket :noncopyable
{
public:
	typedef enum
	{
		NIO,
		BIO
	} IO;

	explicit Socket(int sockfd)
		:m_sockfd(sockfd)
	{

	}

	~Socket();

	/// <summary>
	/// 创建监听文件描述符
	/// </summary>
	/// <param name="io">IO类型:BIO 阻塞，NIO 非阻塞</param>
	/// <param name="family">协议族版本</param>
	/// <returns>fd 错误返回-1 并抛出异常</returns>
	static int create(sa_family_t family, IO type/*=NIO*/);

	/// <summary>
	/// 文件描述符
	/// </summary>
	/// <returns></returns>
	int fd()
	{
		return m_sockfd;
	}

	static void fromIpPort(std::string ip, in_port_t port, sockaddr_in* addr);
	static void fromIpPort(std::string ip, in_port_t port, sockaddr_in6* addr);

private:
	int m_sockfd;
};

namespace Endian
{
	/// <summary>
	/// 主机转大端 16位
	/// </summary>
	/// <param name="host16">16位主机字节序</param>
	/// <returns>16位大端字节序</returns>
	inline uint16_t HostToNetwork16(uint16_t host16)
	{
		return ::htobe16(host16);
	}
	/// <summary>
	/// 主机转大端 32位
	/// </summary>
	/// <param name="host32">32位主机字节序</param>
	/// <returns>32位大端字节序</returns>
	inline uint32_t HostToNetwork32(uint32_t host32)
	{
		return ::htobe32(host32);
	}
	/// <summary>
	/// 主机转大端 64位
	/// </summary>
	/// <param name="host64">64位主机字节序</param>
	/// <returns>64位大端字节序</returns>
	inline uint64_t HostToNetwork64(uint64_t host64)
	{
		return ::htobe64(host64);
	}
	/// <summary>
	/// 大端转主机
	/// </summary>
	/// <param name="network16">16位大端字节序</param>
	/// <returns>16位主机字节序</returns>
	inline uint16_t NetworkToHost(uint16_t network16)
	{
		return ::be16toh(network16);
	}
	/// <summary>
	/// 大端转主机
	/// </summary>
	/// <param name="network32">32位大端字节序</param>
	/// <returns>32位主机字节序</returns>
	inline uint32_t NetworkToHost(uint32_t network32)
	{
		return ::be32toh(network32);
	}
	/// <summary>
	/// 大端转主机
	/// </summary>
	/// <param name="network64">64位大端字节序</param>
	/// <returns>64位主机字节序</returns>
	inline uint64_t NetworkToHost(uint64_t network64)
	{
		return ::be64toh(network64);
	}
	/// <summary>
	/// 是否为小端字节序
	/// </summary>
	/// <returns></returns>
	bool isLittleEndian()
	{
		static union
		{
			short _s = 0x0102;
			char _cc[sizeof(short)];
		};
		return (_cc[0] == 0x02 && _cc[1] == 0x01);
	}
	/// <summary>
	/// 是否为大端字节序
	/// </summary>
	/// <returns></returns>
	bool isBigEndian()
	{
		static union
		{
			short _s = 0x0102;
			char _cc[sizeof(short)];
		};
		return (_cc[0] == 0x01 && _cc[1] == 0x02);
	}
}

