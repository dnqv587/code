#pragma once
#include "../base/noncopyable.h"
#include "InetAddress.h"
#include <functional>
#include <memory>

class EventLoop;
class Channel;
class Connctor :noncopyable,public std::enable_shared_from_this<Connctor>
{
public:
	using NewConnectionCallback = std::function<void(int)>;
	Connctor(EventLoop* loop, const InetAddress& serverAddr);

	~Connctor();

	void start();

	void restart();

	void stop();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{
		m_newConnectionCallback = cb;
	}

private:
	enum States
	{
		kDisconnected,
		kConnecting,
		KConnected
	};

	void setState(States state)
	{
		m_state = state;
	}
	void startInLoop();
	void stopInLoop();
	void connect();
	void connecting(int sockfd);
	void handleWrite();
	void handleError();
	void retry(int sockfd);
	int removeAndResetChannel();
	void resetChannel();

	EventLoop* m_loop;
	InetAddress m_serverAddr;
	States m_state;
	bool m_connect;//保证start和stop存在原子性
	std::unique_ptr<Channel> m_channel;
	int m_retryDelayMs;//重连等待时间-Ms

	NewConnectionCallback m_newConnectionCallback;
};