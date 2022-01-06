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

#define TIMEOUT 10000

using namespace std;

class TcpSocket
{
public:
	TcpSocket();
	TcpSocket(int fd) { m_sock = fd; }
	~TcpSocket();

	//发送报文
	int TcpSend(string sendMsg, size_t timeout = TIMEOUT);
	//接收报文
	string TcpRecv( int timeout=TIMEOUT);
	//创建链接
	int TcpConnect(string ip, unsigned short port, int timeout = TIMEOUT);

private:
	//设置为阻塞
	int setBlock(int fd);
	//设置为非阻塞
	int setNonBlock(int fd);
	//写超时检测
	int  sendTimeout(size_t timeout = TIMEOUT);
	//读超时检测
	int recvTimeout(size_t timeout = TIMEOUT);
	//连接超时检测
	int connectTimeout(struct sockaddr_in *addr, size_t timeout = TIMEOUT);
	//写数据
	int writen(const void* buf, unsigned int len);
	//读数据
	int readn(const void* buf, unsigned int len);
private:
	int m_sock;
};


class connectPoll
{
public:
	connectPoll(string ip, unsigned short port, int number);
	~connectPoll();

	//获取一个连接
	int getConnect();
	//回收一个连接
	void putConnect(int fd);

private:
	//创建服务器
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

	//接受连接---创建服务器链接
	TcpSocket* TcpCreate(int timeout = TIMEOUT);

private:
	int m_listenFd;//监听文件描述符
	TcpSocket* m_tcpScok;//TcpSocket对象---用于通信
};


class TcpClient
{
public:
	TcpClient();
	~TcpClient();

	//创建连接池，并获取一个连接
	TcpSocket* TcpConnectPoll(string ip, unsigned short port, int pollSize = 5, int timeout = TIMEOUT);
	//创建一个链接
	TcpSocket* TcpConnect(string ip, unsigned short port, int timeout = TIMEOUT);

private:
	connectPoll* m_poll;//连接池
	TcpSocket* m_tcpSock;//通信对象
};