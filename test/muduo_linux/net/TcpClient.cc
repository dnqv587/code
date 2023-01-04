#include "TcpClient.h"
#include "../logger/logging.h"
#include "Connector.h"

TcpClient::TcpClient(EventLoop* loop, const InetAddress& addr, const std::string& name)
	:m_loop(CHECK_NOTNULL(loop)),
	m_connector(std::make_shared<Connector>(m_loop,addr)),
	m_name(name),
	m_connect(false)
{
	m_connector->setNewConnectionCallback(std::bind(&TcpClient::newConnection, this, std::placeholders::_1));

	LOG_INFO << "TcpClient::TcpClient [" << this->m_name << "]";
}

void TcpClient::connect()
{
	assert(!m_connect);
	LOG_INFO << " TcpClient::connect [" << m_name << "] - connect to " << m_connector->serverAddress().ipString() << ":" << m_connector->serverAddress().port();
	m_connect = true;
	m_connector->start();
}

void TcpClient::stop()
{
	LOG_INFO << " TcpClient::stop [" << m_name << "] - stop to " << m_connector->serverAddress().ipString() << ":" << m_connector->serverAddress().port();
	m_connect = false;
	m_connector->stop();
}

void TcpClient::newConnection(int sockfd)
{
	
}
