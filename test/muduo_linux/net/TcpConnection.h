#pragma once
#include "InetAddress.h"
#include "../time/Timestamp.h"
#include "../base/noncopyable.h"
#include <string>
#include <memory>
#include <functional>


class EventLoop;
class Socket;
class Channel;
//class Buffer;
/// <summary>
/// TCP连接类，存在时间为建立TCP连接到断开TCP连接
/// </summary>
class TcpConnection :noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
	using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
	typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
	typedef std::function<void(const TcpConnectionPtr&, const char*, int n)> MessageCallback;

	TcpConnection(EventLoop* loop,const std::string& name,int sockfd,const InetAddress& localAddr,const InetAddress& peerAddr);

	~TcpConnection();

	void setConnectionCallback(ConnectionCallback cb)
	{
		m_connectionCallback = cb;
	}

	void setMessageCallback(MessageCallback cb)
	{
		m_messageCallback = cb;
	}

	void connectEstablished();

	std::string name() const
	{
		return m_name;
	}

	InetAddress localAddr() const
	{
		return m_localAddr;
	}

	InetAddress peerAddr() const
	{
		return m_peerAddr;
	}

	bool connected()
	{
		return m_state == kConnected;
	}


private:
	enum StateE
	{
		kConnecting,
		kConnected
	};
	/// <summary>
	/// 设置TcpConnection的状态
	/// </summary>
	/// <param name="s"></param>
	void setState(StateE s)
	{
		m_state = s;
	}

	void handleRead();
	void handleWrite();
	void handleError();

	EventLoop* m_loop;
	std::string m_name;
	StateE m_state;//当前状态
	std::shared_ptr<Socket> m_socket;
	std::shared_ptr<Channel> m_channel;
	InetAddress m_localAddr;
	InetAddress m_peerAddr;
	ConnectionCallback m_connectionCallback;
	MessageCallback m_messageCallback;
};

