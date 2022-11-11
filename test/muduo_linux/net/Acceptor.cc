#include "Acceptor.h"
#include "../event/EventLoop.h"
#include <sys/socket.h>
#include <functional>

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
	:m_loop(loop),
	m_acceptSocket(Socket::create(AF_INET, Socket::NIO)),
	m_acceptChannel(m_loop, m_acceptSocket.fd()),
	m_listenning(false)
{
	m_acceptSocket.setReuseAddr(true);
	m_acceptSocket.bindAddress(listenAddr);
	m_acceptChannel.setReadCallback(std::bind(Acceptor::handleRead, this));
}

void Acceptor::listen()
{
	m_loop->assertInLoopThread();
	m_listenning = true;
	m_acceptSocket.listen();
	m_acceptChannel.enableReading();
}

void Acceptor::handleRead()
{

}
