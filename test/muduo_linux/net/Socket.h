#pragma once
#include "../base/noncopyable.h"
#include <bits/sockaddr.h>
#include <endian.h>
#include <stdint.h>
#include <arpa/inet.h>

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

private:
	int m_sockfd;
};

/*
namespace Endian
{
	uint16_t HostToNetwork16(uint16_t host16)
	{
		//return ::htobe16(host16);
		uint16_t ret;
		uint8_t* data = (uint8_t*)&ret;
		data[0] = host16 & 0x0F;
		data[1] = host16 & 0xF0;
		return ret;
	}

	inline uint32_t HostToNetwork32(uint32_t host32)
	{
		//return ::htobe32(host32);

	}
}*/

