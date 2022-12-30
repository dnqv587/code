#pragma once
#include "../base/noncopyable.h"
#include "../time/Timestamp.h"
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <atomic>

class EventLoop;
class InetAddress;
class Acceptor;
class TcpConnection;
class Buffer;
class EventLoopThreadPool;

/// <summary>
/// 用来管理accept获得的TcpConnection,用户使用
/// </summary>
class TcpServer :noncopyable
{
public:
	using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
	typedef std::function<void(const TcpConnectionPtr&, Buffer*, Timestamp)> MessageCallback;
	typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
	using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;
	using ThreadInitCallback = std::function<void(EventLoop*)>;

	TcpServer(EventLoop* loop, std::string name ,const InetAddress& listenAddr);

	~TcpServer();

	void start();

	/// <summary>
	/// 设置loop线程数
	/// </summary>
	/// <param name="numThreads"></param>
	void setThreadNum(int numThreads);

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

	void setThreadInitCallback(const ThreadInitCallback& cb)
	{
		m_threadInitCallback = cb;
	}

private:
	/// <summary>
	/// 新连接到达，Acceptor调用此回调，创建TcpConnection，加入ConnectionMap
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="peerAddr"></param>
	void newConnection(int sockfd, const InetAddress& peerAddr);

	void removeConnection(const TcpConnectionPtr& conn);

	void removeConnectionInLoop(const TcpConnectionPtr& conn);

	using ConnectionMap = std::map<std::string, TcpConnectionPtr>;
	EventLoop* m_loop;
	const std::string m_name;
	std::unique_ptr<Acceptor> m_acceptor;
	std::atomic<bool> m_started;
	int m_nextConnId;
	ConnectionMap m_connections;
	std::unique_ptr<EventLoopThreadPool> m_threadPool;

	MessageCallback m_messageCallback;
	ConnectionCallback m_connetionCallback;
	WriteCompleteCallback m_writeCompleteCallback;
	ThreadInitCallback m_threadInitCallback;
};

