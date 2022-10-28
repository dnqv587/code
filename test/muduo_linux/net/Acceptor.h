#pragma once 
#include "../base/noncopyable.h"
#include "Socket.h"
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

	void listen();

private:
	void handleRead();

	EventLoop* m_loop;
	NewConnectionCallback m_newConnectionCallback;
	bool m_listenning;
	Socket m_acceptSocket;
};
