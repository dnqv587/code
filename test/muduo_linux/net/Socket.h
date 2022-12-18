#pragma once
#include "../base/noncopyable.h"
#include "../base/Type.h"
#include <bits/sockaddr.h>
#include <endian.h>
#include <stdint.h>
#include <string>
#include <netinet/in.h>
#include <sys/types.h>


class InetAddress;
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

	/// <summary>
	/// 设置重用地址
	/// </summary>
	/// <param name="on">是否启用</param>
	void setReuseAddr(bool on);
	/// <summary>
	/// 绑定地址
	/// </summary>
	/// <param name="addr">InetAddress对象</param>
	void bindAddress(const InetAddress& addr);

	/// <summary>
	/// 监听
	/// </summary>
	void listen();
	/// <summary>
	/// accept封装函数
	/// </summary>
	/// <param name="peerAddr">传出地址</param>
	/// <returns></returns>
	int accept(InetAddress* peerAddr);

	/// <summary>
	/// 关闭文件描述符
	/// </summary>
	/// <param name="sockfd"></param>
	static void close(int sockfd);
	void close();

	/// <summary>
	/// 获取sockfd错误号
	/// </summary>
	/// <returns></returns>
	int getSocketError();
	static int getSocketError(int sockfd);

	ssize_t read(void* buf, size_t count);
	static ssize_t read(int sockfd, void* buf, size_t count);
	ssize_t readv(const struct iovec* iov, int iovcnt);
	static ssize_t readv(int sockfd,const struct iovec* iov, int iovcnt);

	ssize_t write(void* buf, size_t count);
	static ssize_t write(int sockfd, void* buf, size_t count);

	/// <summary>
	/// 解析字符串类型点分十进制IP地址为网络IP
	/// </summary>
	/// <param name="ip">String类型的点分十进制IP</param>
	/// <param name="port">端口号</param>
	/// <param name="addr">IPV4 socket地址 [IO]</param>
	static void fromIpPort(std::string& ip, in_port_t port, sockaddr_in* addr);
	/// <summary>
	/// 解析字符串类型点分十进制IP地址为网络IP
	/// </summary>
	/// <param name="ip">String类型的点分十进制IP</param>
	/// <param name="port">端口号</param>
	/// <param name="addr">IPV6 socket地址 [IO]</param>
	static void fromIpPort(std::string& ip, in_port_t port, sockaddr_in6* addr);

	/// <summary>
	/// 网络IP格式化为点分十进制IP
	/// </summary>
	/// <param name="addr">socket地址</param>
	/// <returns></returns>
	static std::string toIpString(struct sockaddr* addr);
	static std::string toIpString(const struct sockaddr* addr);

	/// <summary>
	/// 获取本端sock地址
	/// </summary>
	/// <param name="sockfd"></param>
	/// <returns></returns>
	static struct sockaddr_in6 getLocalAddr(int sockfd);

	/// <summary>
	/// 获取远端sock地址
	/// </summary>
	/// <param name="sockfd"></param>
	/// <returns></returns>
	static struct sockaddr_in6 getPeerAddr(int sockfd);

	/// <summary>
	/// sockaddr_in转sockaddr
	/// </summary>
	static const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr)
	{
		return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
	}
	static struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr6)
	{
		return static_cast<struct sockaddr*>(implicit_cast<void*>(addr6));
	}
	static const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr6)
	{
		return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr6));
	}
	/// <summary>
	/// sockaddr转sockaddr_in
	/// </summary>
	static const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr)
	{
		return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
	}
	/// <summary>
	/// sockaddr转sockaddr_in6
	/// </summary>
	static const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr)
	{
		return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
	}
	//设置sockfd属性为O_NONBLOCK | FD_CLOEXEC
	static void setNonBlockAndCloseOnExec(int sockfd);

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
	inline uint16_t NetworkToHost16(uint16_t network16)
	{
		return ::be16toh(network16);
	}
	/// <summary>
	/// 大端转主机
	/// </summary>
	/// <param name="network32">32位大端字节序</param>
	/// <returns>32位主机字节序</returns>
	inline uint32_t NetworkToHost32(uint32_t network32)
	{
		return ::be32toh(network32);
	}
	/// <summary>
	/// 大端转主机
	/// </summary>
	/// <param name="network64">64位大端字节序</param>
	/// <returns>64位主机字节序</returns>
	inline uint64_t NetworkToHost64(uint64_t network64)
	{
		return ::be64toh(network64);
	}
	/// <summary>
	/// 是否为小端字节序
	/// </summary>
	/// <returns></returns>
	bool isLittleEndian();
	/// <summary>
	/// 是否为大端字节序
	/// </summary>
	/// <returns></returns>
	bool isBigEndian();
}

