#pragma once
#include "../base/noncopyable.h"
#include "../time/Timestamp.h"
#include <string>
#include <memory>
#include <map>
#include <functional>

class EventLoop;
class InetAddress;
class Acceptor;
class TcpConnectionPtr;
class Buffer;
/// <summary>
/// 用来管理accept获得的TcpConnection
/// </summary>
class TcpServer :noncopyable
{
public:
	typedef std::function<void(const TcpConnectionPtr&,Buffer*,Timestamp)> MessageCallback;
	typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;

	TcpServer(EventLoop* loop, const InetAddress& listenAddr);

	~TcpServer();

	void start();

	void setConnectionCallback(ConnectionCallback& cb)
	{
		m_connetionCallback = cb;
	}

	void setMessageCallback(MessageCallback& cb)
	{
		m_messageCallback = cb;
	}


private:
	using ConnectionMap = std::map<std::string, TcpConnectionPtr>;
	EventLoop* m_loop;
	const std::string m_name;
	std::unique_ptr<Acceptor> m_acceptor;
	bool m_started;
	int m_nextConnId;
	ConnectionMap m_connections;
	MessageCallback m_messageCallback;
	ConnectionCallback m_connetionCallback;
};

