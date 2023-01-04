#include "TcpClient.h"
#include "../logger/logging.h"
#include "Connector.h"
#include "TcpConnection.h"
#include "../event/EventLoop.h"

namespace detail
{
	static void removeConnection(EventLoop* loop, const std::shared_ptr<TcpConnection>& conn)
	{
		loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
	}
}

TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr, const std::string& name)
	:m_loop(CHECK_NOTNULL(loop)),
	m_connector(std::make_shared<Connector>(m_loop, serverAddr)),
	m_name(name),
	m_connect(true),
	m_retry(false),
	m_nextID(1)
{
	m_connector->setNewConnectionCallback(std::bind(&TcpClient::newConnection, this, std::placeholders::_1));

	LOG_INFO << "TcpClient::TcpClient [" << this->m_name << "]";
}

TcpClient::~TcpClient()
{
	LOG_INFO << "TcpClient::~TcpClient [" << m_name << "]";
	bool unique = false;
	TcpConnectionPtr conn;
	{
		MutexLockGuard lock(m_mutex);
		unique = m_connection.unique();
		conn = m_connection;
	}
	if (conn)
	{
		assert(m_loop == conn->getLoop());
		std::function<void(const TcpConnectionPtr&)> cb = std::bind(&detail::removeConnection, m_loop, std::placeholders::_1);
		m_loop->runInLoop(std::bind(&TcpConnection::setCloseCallbck, conn, cb));
		if (unique)
		{
			conn->forceClose();
		}
	}
	else
	{
		m_connect = false;
		m_connector->stop();
	}
}

void TcpClient::connect()
{
	LOG_INFO << " TcpClient::connect [" << m_name << "] - connect to " << m_connector->serverAddress().ipString() << ":" << m_connector->serverAddress().port();
	m_connect = true;
	m_connector->start();
}

void TcpClient::disconnect()
{
	m_connect = false;
	{
		MutexLockGuard lock(m_mutex);
		if (m_connection)
		{
			m_connection->shutdown();
		}
	}
}

void TcpClient::stop()
{
	LOG_INFO << " TcpClient::stop [" << m_name << "] - stop to " << m_connector->serverAddress().ipString() << ":" << m_connector->serverAddress().port();
	m_connect = false;
	m_connector->stop();
}

void TcpClient::newConnection(int sockfd)
{
	m_loop->assertInLoopThread();
	InetAddress peerAddr(Socket::getPeerAddr(sockfd));
	char buf[32];
	snprintf(buf, sizeof buf, ":%s:%d#%d", peerAddr.ipString(), peerAddr.port(), m_nextID);
	++m_nextID;
	std::string connName = m_name + buf;
	InetAddress localAddr(Socket::getLocalAddr(sockfd));

	TcpConnectionPtr conn(std::make_shared<TcpConnection>(m_loop, connName, sockfd, localAddr, peerAddr));

	conn->setConnectionCallback(m_connetionCallback);
	conn->setMessageCallback(m_messageCallback);
	conn->setWriteCompleteCallback(m_writeCompleteCallback);
	conn->setCloseCallbck(std::bind(&TcpClient::removeConnection, this, std::placeholders::_1));
	{
		MutexLockGuard lock(m_mutex);
		m_connection = conn;
	}
	conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
	m_loop->assertInLoopThread();
	assert(m_loop == conn->getLoop());
	{
		MutexLockGuard lock(m_mutex);
		assert(conn == m_connection);
		m_connection.reset();
	}
	m_loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
	if (m_retry && m_connect)
	{
		LOG_INFO << "TcpClient::connect[" << m_name << "] -reconnecting to " << m_connector->serverAddress().ipString() << ":" << m_connector->serverAddress().port();
		m_connector->restart();
	}
}
