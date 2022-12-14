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
class TcpConnection;
class Buffer;

/// <summary>
/// 用来管理accept获得的TcpConnection,用户使用
/// </summary>
class TcpServer :noncopyable
{
public:
	using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
	typedef std::function<void(const TcpConnectionPtr&, Buffer*, Timestamp)> MessageCallback;
	typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;

	TcpServer(EventLoop* loop, std::string name ,const InetAddress& listenAddr);

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
	/// <summary>
	/// 新连接到达，Acceptor调用此回调，创建TcpConnection，加入ConnectionMap
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="peerAddr"></param>
	void newConnection(int sockfd, const InetAddress& peerAddr);

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

