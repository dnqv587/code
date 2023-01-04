#pragma once
#include "InetAddress.h"
#include "../time/Timestamp.h"
#include "../base/noncopyable.h"
#include "../base/Buffer.h"
#include <string>
#include <memory>
#include <functional>


class EventLoop;
class Socket;
class Channel;
/// <summary>
/// TCP连接类，存在时间为建立TCP连接到断开TCP连接
/// </summary>
class TcpConnection :noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
	using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
	typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
	typedef std::function<void(const TcpConnectionPtr&, Buffer*, Timestamp)> MessageCallback;
	using CloseCallback = std::function<void(const TcpConnectionPtr&)>;
	using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;
	using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr&, size_t)>;

	TcpConnection(EventLoop* loop,const std::string& name,int sockfd,const InetAddress& localAddr,const InetAddress& peerAddr);

	~TcpConnection();

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		m_connectionCallback = cb;
	}

	void setMessageCallback(const MessageCallback& cb)
	{
		m_messageCallback = cb;
	}

	void setCloseCallbck(const CloseCallback& cb)
	{
		m_closeCallback = cb;
	}

	void setWriteCompleteCallback(const WriteCompleteCallback& cb)
	{
		m_writeCompleteCallback = cb;
	}

	void setHighWaterMarkCallback(const HighWaterMarkCallback& cb,size_t highWaterMark)
	{
		m_highWaterMarkCallback = cb;
		m_highWaterMark = highWaterMark;
	}

	/// <summary>
	/// 发送msg
	/// --线程安全，inLoop
	/// </summary>
	/// <param name="msg"></param>
	/// <param name="len"></param>
	void send(const void* msg, size_t len);
	void send(const std::string& msg);

	/// <summary>
	/// 关闭连接--写
	/// </summary>
	void shutdown();

	/// <summary>
	/// 禁用Nagle算法 -- 避免连续发包出现延迟
	/// </summary>
	/// <param name="on">开关</param>
	void setTcpNoDelay(bool on);

	/// <summary>
	/// 当TcpServer接受一个新连接时调用
	/// </summary>
	void connectEstablished();

	/// <summary>
	/// 断开连接时，TcpServer移除的此TcpConnectionPtr
	/// </summary>
	void connectDestroyed();

	/// <summary>
	/// 强制关闭连接
	/// </summary>
	void forceClose();

	/// <summary>
	/// 连接名
	/// </summary>
	/// <returns></returns>
	std::string name() const
	{
		return m_name;
	}

	/// <summary>
	/// 返回本地InetAddress地址
	/// </summary>
	/// <returns></returns>
	InetAddress localAddr() const
	{
		return m_localAddr;
	}

	/// <summary>
	/// 返回远端InetAddress地址
	/// </summary>
	/// <returns></returns>
	InetAddress peerAddr() const
	{
		return m_peerAddr;
	}

	/// <summary>
	/// 当前是否已连接
	/// </summary>
	/// <returns></returns>
	bool connected()
	{
		return m_state == kConnected;
	}

	EventLoop* getLoop()
	{
		return m_loop;
	}

private:
	enum StateE
	{
		kConnecting,
		kConnected,
		kDisconnecting,
		kDisconnected
	};
	/// <summary>
	/// 设置TcpConnection的状态
	/// </summary>
	/// <param name="s"></param>
	void setState(StateE s)
	{
		m_state = s;
	}

	void handleRead(Timestamp receiveTime);
	void handleError();
	void handleClose();
	void handleWrite();
	void sendInLoop(const void* data,size_t len);
	void shutdownInLoop();
	void forceCloseInLoop();

	EventLoop* m_loop;
	std::shared_ptr<Socket> m_socket;
	std::shared_ptr<Channel> m_channel;

	InetAddress m_localAddr;
	InetAddress m_peerAddr;
	std::string m_name;
	StateE m_state;//当前状态
	size_t m_highWaterMark;//高水位限值

	Buffer m_inputBuffer;
	Buffer m_outputBuffer;

	//回调
	ConnectionCallback m_connectionCallback;
	MessageCallback m_messageCallback;
	CloseCallback m_closeCallback;
	WriteCompleteCallback m_writeCompleteCallback;
	HighWaterMarkCallback m_highWaterMarkCallback;
};

