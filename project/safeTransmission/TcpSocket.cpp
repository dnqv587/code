#include "TcpSocket.h"

TcpSocket::TcpSocket()
{
#ifdef WIN32
	m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	m_sock = socket(PF_INET, SOCK_STREAM, 0);
#endif
}

TcpSocket::~TcpSocket()
{
#ifdef WIN32
	closesocket(m_sock);
#else
	close(m_sock);
#endif
}


int TcpSocket::TcpSend(string sendMsg, size_t timeout )
{
	int ret = sendTimeout(timeout);
	if (ret == 0)
	{

		int writed = 0;
		int dataLen = sendMsg.size() + 4;
		// 添加的4字节作为数据头, 存储数据块长度
		unsigned char* netdata = (unsigned char*)malloc(dataLen);
		if (netdata == NULL)
		{
			ret = -1;
			printf("func sckClient_send() mlloc Err:%d\n ", ret);
			return ret;
		}
		// 转换为网络字节序
		int netlen = htonl(sendMsg.size());
		memcpy(netdata, &netlen, 4);
		memcpy(netdata + 4, sendMsg.data(), sendMsg.size());

		// 没问题返回发送的实际字节数, 应该 == 第二个参数: dataLen
		// 失败返回: -1
		writed = writen(netdata, dataLen);
		if (writed < dataLen)	// 发送失败
		{
			if (netdata != NULL)
			{
				free(netdata);
				netdata = NULL;
			}
			return writed;
		}
		// 释放内存
		if (netdata != NULL)
		{
			free(netdata);
			netdata = NULL;
		}
	}
	else
	{
		//失败返回-1，超时返回-1并且errno = ETIMEDOUT
		if (ret == -1 && errno == ETIMEDOUT)
		{
			ret = -1;
			printf("func sckClient_send() mlloc Err:%d\n ", ret);
		}
	}

	return ret;
}

string TcpSocket::TcpRecv( int timeout)
{
	int ret = recvTimeout(timeout);
	if (ret != 0)
	{
		if (ret == -1 || errno == ETIMEDOUT)
		{
			printf("readTimeout(timeout) err: TimeoutError \n");
			return string();
		}
		else
		{
			printf("readTimeout(timeout) err: %d \n", ret);
			return string();
		}
	}

	int netdatalen = 0;
	ret = readn(&netdatalen, 4); //读包头 4个字节
	if (ret == -1)
	{
		printf("func readn() err:%d \n", ret);
		return string();
	}
	else if (ret < 4)
	{
		printf("func readn() err peer closed:%d \n", ret);
		return string();
	}

	int n = ntohl(netdatalen);
	// 根据包头中记录的数据大小申请内存, 接收数据
	char* tmpBuf = (char*)malloc(n + 1);
	if (tmpBuf == NULL)
	{
		ret = -1;
		printf("malloc() err \n");
		return NULL;
	}

	ret = readn(tmpBuf, n); //根据长度读数据
	if (ret == -1)
	{
		printf("func readn() err:%d \n", ret);
		return string();
	}
	else if (ret < n)
	{
		printf("func readn() err peer closed:%d \n", ret);
		return string();
	}

	tmpBuf[n] = '\0'; //多分配一个字节内容，兼容可见字符串 字符串的真实长度仍然为n
	string data = string(tmpBuf);
	// 释放内存
	free(tmpBuf);

	return data;
}

int TcpSocket::TcpConnect(string ip, unsigned short port, int timeout /*= TIMEOUT*/)
{
#ifdef WIN32
	int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	int fd = socket(PF_INET, SOCK_STREAM, 0);
#endif
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
#ifdef WIN32
	inet_pton(AF_INET, ip.c_str(), (PVOID)&addr.sin_addr.s_addr);
#else
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);
#endif
	//connect(fd, (struct sockaddr*)&addr, sizeof(addr));

	return connectTimeout(&addr, timeout);
}

int TcpSocket::setBlock(int fd)
{
	int ret = 0;

#ifdef WIN32
	unsigned long ul = 0;
	ret = ioctlsocket(fd, FIONBIO, &ul); //设置为阻塞模式

#else
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1)
	{
		return flags;
	}

	flags &= ~O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, flags);
#endif
	return ret;
}

int TcpSocket::setNonBlock(int fd)
{
	int ret = 0;

#ifdef WIN32
	unsigned long ul = 1;
	ret = ioctlsocket(fd, FIONBIO, &ul); //设置为非阻塞模式

#else
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1)
	{
		return flags;
	}

	flags |= O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, flags);
#endif
	return ret;
}

int TcpSocket::sendTimeout(size_t timeout /*= TIMEOUT*/)
{
	int ret = 0;
	if (timeout > 0)
	{
		struct timeval timeoutVal;
		timeoutVal.tv_sec = timeout;
		timeoutVal.tv_usec = 0;

		fd_set send_fdset;
		FD_ZERO(&send_fdset);
		FD_SET(m_sock, &send_fdset);
		do
		{
			ret = select(m_sock + 1,NULL , &send_fdset, NULL, &timeoutVal);
		} while (ret < 0 && errno == EINTR);//被信号打断，继续
		if (ret == 0)//读事件超时
		{
			ret = -1;
			errno = ETIMEDOUT;
		}
		else
		{
			ret = 0;
		}
	}
	return ret;
}

int TcpSocket::recvTimeout(size_t timeout /*= TIMEOUT*/)
{
	int ret = 0;
	if (timeout > 0)
	{
		struct timeval timeoutVal;
		timeoutVal.tv_sec = timeout;
		timeoutVal.tv_usec = 0;

		fd_set recv_fdset;
		FD_ZERO(&recv_fdset);
		FD_SET(m_sock, &recv_fdset);
		do
		{
			ret = select(m_sock + 1, &recv_fdset,NULL , NULL, &timeoutVal);
		} while (ret < 0 && errno == EINTR);//被信号打断，继续
		if (ret == 0)//读事件超时
		{
			ret = -1;
			errno = ETIMEDOUT;
		}
		else
			ret = 0;
	}
	return ret;
}

int TcpSocket::connectTimeout(struct sockaddr_in *addr, size_t timeout /*= TIMEOUT*/)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (timeout > 0)
	{
		setNonBlock(m_sock);	// 设置非阻塞IO
	}

	ret = connect(m_sock, (struct sockaddr*)addr, addrlen);
	// 非阻塞模式连接, 返回-1, 并且errno为EINPROGRESS, 表示连接正在进行中
	if (ret < 0 && errno == EINPROGRESS)
	{
		fd_set connect_fdset;
		struct timeval timeoutVal;
		FD_ZERO(&connect_fdset);
		FD_SET(m_sock, &connect_fdset);
		timeoutVal.tv_sec = timeout;
		timeoutVal.tv_usec = 0;
		do
		{
			// 一但连接建立，则套接字就可写 所以connect_fdset放在了写集合中
			ret = select(m_sock + 1, NULL, &connect_fdset, NULL, &timeoutVal);
		} while (ret < 0 && errno == EINTR);

		if (ret == 0)
		{
			// 超时
			ret = -1;
			errno = ETIMEDOUT;
		}
		else if (ret < 0)
		{
			return -1;
		}
		else if (ret == 1)
		{
			/* ret返回为1（表示套接字可写），可能有两种情况，一种是连接建立成功，一种是套接字产生错误，*/
			/* 此时错误信息不会保存至errno变量中，因此，需要调用getsockopt来获取。 */
			char err;
			socklen_t sockLen = sizeof(err);
			int sockoptret = getsockopt(m_sock, SOL_SOCKET, SO_ERROR, &err, &sockLen);
			if (sockoptret == -1)
			{
				return -1;
			}
			if (err == 0)
			{
				ret = 0;	// 成功建立连接
			}
			else
			{
				// 连接失败
				errno = err;
				ret = -1;
			}
		}
	}
	if (timeout > 0)
	{
		setBlock(m_sock);	// 套接字设置回阻塞模式
	}
	return ret;
}

int TcpSocket::writen(const void* buf, unsigned int len)
{
	size_t nleft = len;
	ssize_t nwritten;
	char* bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nwritten = write(m_sock, bufp, nleft)) < 0)
		{
			if (errno == EINTR)	// 被信号打断
			{
				continue;
			}
			return -1;
		}
		else if (nwritten == 0)
		{
			continue;
		}

		bufp += nwritten;
		nleft -= nwritten;
}

	return len;
}

int TcpSocket::readn(const void* buf, unsigned int len)
{
	size_t nleft = len;
	ssize_t nread;
	char* bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nread = read(m_sock, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			return -1;
		}
		else if (nread == 0)
		{
			return len - nleft;
		}

		bufp += nread;
		nleft -= nread;
}

	return len;
}

TcpServer::TcpServer():m_listenFd(0),m_tcpScok(nullptr)
{
#ifdef WIN32
	m_listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	m_listenFd = socket(PF_INET, SOCK_STREAM, 0);
#endif

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_listenFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		perror("bind error");
	listen(m_listenFd, 1024);
}

TcpServer::TcpServer(int port):m_listenFd(0), m_tcpScok(nullptr)
{
#ifdef WIN32
	m_listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	m_listenFd = socket(PF_INET, SOCK_STREAM, 0);
#endif

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_listenFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		perror("bind error");
	listen(m_listenFd, 1024);

}

TcpServer::~TcpServer()
{

	delete m_tcpScok;

#ifdef WIN32
	closesocket(m_listenFd);
#else
	close(m_listenFd);
#endif
}

TcpSocket* TcpServer::TcpCreate(int timeout )
{
	int fd = accept(m_listenFd, NULL, NULL);
	m_tcpScok = new TcpSocket(fd);
	if (m_tcpScok != NULL)
	{
		return m_tcpScok;
	}
	return NULL;
}

TcpClient::TcpClient()
{
	m_poll = nullptr;
	m_tcpSock = nullptr;
}

TcpClient::~TcpClient()
{
	delete m_tcpSock;
	delete m_poll;
}

TcpSocket* TcpClient::TcpConnectPoll(string ip, unsigned short port, int pollSize, int timeout)
{
	m_poll = new connectPoll(ip, port, pollSize);

	int fd = m_poll->getConnect();
	m_tcpSock = new TcpSocket(fd);
	if (m_tcpSock != NULL)
	{
		return m_tcpSock;
	}
	return NULL;
}

TcpSocket* TcpClient::TcpConnect(string ip, unsigned short port, int timeout /*= TIMEOUT*/)
{
	m_tcpSock = new TcpSocket;
	m_tcpSock->TcpConnect(ip, port, timeout);
	if (m_tcpSock != NULL)
	{
		return m_tcpSock;
	}
	return NULL;
}

connectPoll::connectPoll(string ip, unsigned short port, int number):m_ip(ip),m_port(port),m_number(number)
{
	for (int i = 0; i < number; ++i)
	{
#ifdef WIN32
		int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
		int fd = socket(PF_INET, SOCK_STREAM, 0);
#endif
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
#ifdef WIN32
		inet_pton(AF_INET, ip.c_str(), (PVOID)&addr.sin_addr.s_addr);
#else
		inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);
#endif
		connect(fd, (struct sockaddr*)&addr, sizeof(addr));

		m_listFd.push(fd);

	}
}

connectPoll::~connectPoll()
{
	for (auto iter = m_listFd.front(); iter != m_listFd.back(); ++iter)
	{
#ifdef WIN32
		closesocket(iter);
#else
		close(iter);
#endif
	}
}

int connectPoll::getConnect()
{
	if (m_listFd.size() > 0)
	{
		int fd = m_listFd.front();
		m_listFd.pop();
		return fd;
	}
	return -1;
}

void connectPoll::putConnect(int fd)
{
	m_listFd.push(fd);
}

void connectPoll::createConnect(string ip, unsigned short port, int number)
{

}

