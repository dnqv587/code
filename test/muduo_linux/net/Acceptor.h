#pragma once 
#include "../base/noncopyable.h"
#include "Socket.h"
#include "../event/Channel.h"
#include <functional>



class InetAddress;
class EventLoop;
/// <summary>
/// 用于accept新TCP连接，并通过回调通知使用者。
/// 内部class，供TcpServer使用，生命周期由后者控制
/// </summary>
class Acceptor :noncopyable
{
public:
	using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;

	Acceptor(EventLoop* loop, const InetAddress& listenAddr);

	~Acceptor();
	/// <summary>
	/// 设置接受新连接的回调
	/// </summary>
	/// <param name="cb">回调</param>
	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{
		m_newConnectionCallback = cb;
	}
	/// <summary>
	/// 是否在监听
	/// </summary>
	/// <returns></returns>
	bool listenning() const
	{
		return m_listenning;
	}
	/// <summary>
	/// 开始监听
	/// </summary>
	void listen();

private:
	/// <summary>
	/// 读事件回调-处理连接请求
	/// </summary>
	void handleRead();

	EventLoop* m_loop;
	NewConnectionCallback m_newConnectionCallback;//连接回调
	bool m_listenning;
	Socket m_acceptSocket;
	Channel m_acceptChannel;
	int m_idleFd;//占位文件描述符，用来判断文件描述符是否达上限
};
