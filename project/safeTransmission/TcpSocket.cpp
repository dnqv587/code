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
		ret = send(m_sock, sendMsg.c_str(), sendMsg.length(), 0);
	}
	return ret;
}

string TcpSocket::TcpRecv( int timeout)
{
	string data;
	int ret = recvTimeout(timeout);
	if (ret == 0)
	{
		int n = recv(m_sock, (char*)&data, 1024, 0);
	}
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
	connect(fd, (struct sockaddr*)&addr, sizeof(addr));
}

int TcpSocket::setBlock(int fd)
{
	int ret = 0;

#ifdef WIN32
	unsigned long ul = 0;
	ret = ioctlsocket(fd, FIONBIO, &ul); //����Ϊ����ģʽ

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
	ret = ioctlsocket(fd, FIONBIO, &ul); //����Ϊ������ģʽ

#else
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1)
	{
		return flags;
	}

	flags |= O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, flags);
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

		fd_set read_fdset;
		FD_ZERO(&read_fdset);
		FD_SET(m_sock, &read_fdset);
		do
		{
			ret = select(m_sock + 1, &read_fdset, NULL, NULL, &timeoutVal);
		} while (ret < 0 && errno == EINTR);//���źŴ�ϣ�����
		if (ret == 0)//���¼���ʱ
		{
			ret = -1;
			errno = ETIMEDOUT;
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
			ret = select(m_sock + 1, NULL, &recv_fdset, NULL, &timeoutVal);
		} while (ret < 0 && errno == EINTR);//���źŴ�ϣ�����
		if (ret == 0)//���¼���ʱ
		{
			ret = -1;
			errno = ETIMEDOUT;
		}
	}
	return ret;
}

int TcpSocket::connectTimeout(struct sockaddr_in *addr, size_t timeout /*= TIMEOUT*/)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (timeout > 0)
	{
		setNonBlock(m_sock);	// ���÷�����IO
	}

	ret = connect(m_sock, (struct sockaddr*)addr, addrlen);
	// ������ģʽ����, ����-1, ����errnoΪEINPROGRESS, ��ʾ�������ڽ�����
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
			// һ�����ӽ��������׽��־Ϳ�д ����connect_fdset������д������
			ret = select(m_sock + 1, NULL, &connect_fdset, NULL, &timeoutVal);
		} while (ret < 0 && errno == EINTR);

		if (ret == 0)
		{
			// ��ʱ
			ret = -1;
			errno = ETIMEDOUT;
		}
		else if (ret < 0)
		{
			return -1;
		}
		else if (ret == 1)
		{
			/* ret����Ϊ1����ʾ�׽��ֿ�д�������������������һ�������ӽ����ɹ���һ�����׽��ֲ�������*/
			/* ��ʱ������Ϣ���ᱣ����errno�����У���ˣ���Ҫ����getsockopt����ȡ�� */
			char err;
			socklen_t sockLen = sizeof(err);
			int sockoptret = getsockopt(m_sock, SOL_SOCKET, SO_ERROR, &err, &sockLen);
			if (sockoptret == -1)
			{
				return -1;
			}
			if (err == 0)
			{
				ret = 0;	// �ɹ���������
			}
			else
			{
				// ����ʧ��
				errno = err;
				ret = -1;
			}
		}
	}
	if (timeout > 0)
	{
		setBlock(m_sock);	// �׽������û�����ģʽ
	}
	return ret;
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
	int m_listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	int m_listenFd = socket(PF_INET, SOCK_STREAM, 0);
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

