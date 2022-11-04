#include "Acceptor.h"

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
	:m_loop(loop),
	m_acceptSocket(Socket::create(2, Socket::NIO))
{

}

void Acceptor::handleRead()
{

}
