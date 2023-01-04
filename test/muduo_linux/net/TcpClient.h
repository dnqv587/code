#pragma once
#include "../base/noncopyable.h"
#include "TcpConnection.h"


class Connector;
class TcpClient :noncopyable
{
public:
	using ConnectorPtr = std::shared_ptr<Connector>;
	using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
	typedef std::function<void(const TcpConnectionPtr&, Buffer*, Timestamp)> MessageCallback;
	typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
	using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;

	TcpClient(EventLoop* loop, const InetAddress& serverAddr,const std::string& name);
	~TcpClient();

	void connect();
	void disconnect();
	void stop();


	void setConnectionCallback(const ConnectionCallback& cb)
	{
		m_connetionCallback = cb;
	}

	void setMessageCallback(const MessageCallback& cb)
	{
		m_messageCallback = cb;
	}

	void setWriteCompleteCallback(const WriteCompleteCallback& cb)
	{
		m_writeCompleteCallback = cb;
	}

private:
	void newConnection(int sockfd);

	EventLoop* m_loop;
	ConnectorPtr m_connector;
	const std::string m_name;
	bool m_connect;

	MessageCallback m_messageCallback;
	ConnectionCallback m_connetionCallback;
	WriteCompleteCallback m_writeCompleteCallback;
};

