#include "Acceptor.h"
#include "../event/EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "../logger/logging.h"
#include <sys/socket.h>
#include <functional>
#include <fcntl.h>
#include <unistd.h>


Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
	:m_loop(loop),
	m_acceptSocket(Socket::create(AF_INET, Socket::NIO)),
	m_acceptChannel(m_loop, m_acceptSocket.fd()),
	m_listenning(false),
	m_idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
	assert(m_idleFd >= 0);
	m_acceptSocket.setReuseAddr(true);
	m_acceptSocket.bindAddress(listenAddr);
	m_acceptChannel.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
	m_acceptChannel.disableALL();
	m_acceptChannel.remove();
	::close(m_idleFd);
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
	m_loop->assertInLoopThread();
	InetAddress peerAddr(NULL);
	int connfd = m_acceptSocket.accept(&peerAddr);
	if (connfd >= 0)
	{
		if (m_newConnectionCallback)
		{
			m_newConnectionCallback(connfd, peerAddr);
		}
		else
		{
			Socket::close(connfd);
		}
	}
	else
	{
		LOG_SYSERR << "Acceptor::handleRead error";
		//文件描述符资源不足，导致无法申请通信描述符来断开连接
		// 解决：将占位的文件描述符释放，在进行fd
		//非线程安全，因为占位fd只有一个，但可能存在多个线程争抢此fd
		if (errno == EMFILE)
		{
			::close(m_idleFd);
			//断开此连接
			m_idleFd = ::accept(m_acceptSocket.fd(), NULL, NULL);
			::close(m_idleFd);
			
			m_idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
		}

	}
}
