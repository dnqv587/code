#pragma once
#include "../base/noncopyable.h"
#include <bits/sockaddr.h>

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

