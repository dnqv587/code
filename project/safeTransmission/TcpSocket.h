#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <WInSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#pragma comment (lib,"WSOCK32.LIB")
#pragma comment(lib,"Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <fcntl.h>
#endif

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#define TIMEOUT 10000

using namespace std;

class TcpSocket
{
public:
	TcpSocket();
	TcpSocket(int fd) { m_sock = fd; }
	~TcpSocket();

	//���ͱ���
	int TcpSend(string sendMsg, size_t timeout = TIMEOUT);
	//���ձ���
	string TcpRecv( int timeout=TIMEOUT);
	//��������
	int TcpConnect(string ip, unsigned short port, int timeout = TIMEOUT);

private:
	//����Ϊ����
	int setBlock(int fd);
	//����Ϊ������
	int setNonBlock(int fd);
	//д��ʱ���
	int  sendTimeout(size_t timeout = TIMEOUT);
	//����ʱ���
	int recvTimeout(size_t timeout = TIMEOUT);
	//���ӳ�ʱ���
	int connectTimeout(struct sockaddr_in *addr, size_t timeout = TIMEOUT);
	//д����
	int writen(const void* buf, unsigned int len);
	//������
	int readn(const void* buf, unsigned int len);
private:
	int m_sock;
};


class connectPoll
{
public:
	connectPoll(string ip, unsigned short port, int number);
	~connectPoll();

	//��ȡһ������
	int getConnect();
	//����һ������
	void putConnect(int fd);

private:
	//����������
	void createConnect(string ip, unsigned short port, int number);
private:
	queue<int>m_listFd;
	string m_ip;
	unsigned short m_port;
	int m_number;
};

class TcpServer
{
public:
	TcpServer();
	TcpServer(int port);
	~TcpServer();

	//��������---��������������
	TcpSocket* TcpCreate(int timeout = TIMEOUT);

private:
	int m_listenFd;//�����ļ�������
	TcpSocket* m_tcpScok;//TcpSocket����---����ͨ��
};


class TcpClient
{
public:
	TcpClient();
	~TcpClient();

	//�������ӳأ�����ȡһ������
	TcpSocket* TcpConnectPoll(string ip, unsigned short port, int pollSize = 5, int timeout = TIMEOUT);
	//����һ������
	TcpSocket* TcpConnect(string ip, unsigned short port, int timeout = TIMEOUT);

private:
	connectPoll* m_poll;//���ӳ�
	TcpSocket* m_tcpSock;//ͨ�Ŷ���
};