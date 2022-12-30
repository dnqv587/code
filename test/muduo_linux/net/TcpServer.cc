#include "TcpServer.h"
#include "../event/EventLoop.h"
#include "../logger/logging.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "functional"
#include "Socket.h"
#include "TcpConnection.h"
#include "../event/EventLoopThreadPool.h"


TcpServer::TcpServer(EventLoop* loop, std::string name, const InetAddress& listenAddr)
	:m_loop(loop),
	m_name(name),
	m_acceptor(new Acceptor(m_loop, listenAddr)),
	m_started(false),
	m_threadPool(new EventLoopThreadPool(m_loop, m_name)),
	m_nextConnId(1)
{
	//设置accept的回调，并先占位
	m_acceptor->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
	m_loop->assertInLoopThread();

	for (auto& item : m_connections)
	{
		TcpConnectionPtr conn(item.second);
		item.second.reset();
		conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
	}
}

void TcpServer::start()
{
	if (!m_started.exchange(true))
	{
		m_threadPool->start(m_threadInitCallback);
		assert(!m_acceptor->listenning());
		m_loop->runInLoop(std::bind(&Acceptor::listen, m_acceptor.get()));
	}
}

void TcpServer::setThreadNum(int numThreads)
{
	assert(0 <= numThreads);
	m_threadPool->setThreadNum(numThreads);
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
	m_loop->assertInLoopThread();
	char buf[32];
	snprintf(buf, sizeof buf, "#%d", m_nextConnId);
	++m_nextConnId;
	std::string connName = m_name + buf;
	
	LOG_INFO << "TcpServer::newConnection [" << m_name
		<< "] - new connection [" << connName
		<< "] from " << peerAddr.port();
	
	InetAddress localAddr(Socket::getLocalAddr(sockfd));
	EventLoop* ioLoop = m_threadPool->getNextLoop();
	TcpConnectionPtr conn(std::make_shared<TcpConnection>(ioLoop, connName, sockfd, localAddr, peerAddr));
	m_connections[connName] = conn;
	conn->setConnectionCallback(m_connetionCallback);
	conn->setMessageCallback(m_messageCallback);
	conn->setCloseCallbck(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
	conn->setWriteCompleteCallback(m_writeCompleteCallback);
	//conn->connectEstablished();
	ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	m_loop->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	m_loop->assertInLoopThread();
	LOG_INFO << " TcpServer::removeConnection [" << m_name << "] - connection " << conn->name();
	size_t n = m_connections.erase(conn->name());//释放连接并close socket fd
	assert(n == 1);
	EventLoop* ioLoop = m_threadPool->getNextLoop();
	ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

