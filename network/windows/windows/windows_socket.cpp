#include "windows_socket.h"


void service()
{
	//创建socket
	int lfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//TCP
	if (lfd == INVALID_SOCKET)
	{
		perror("socket error");
	}
	struct sockaddr_in serv;
	memset(&serv, 0x00, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	//绑定socket
	int ret = bind(lfd, (struct sockaddr*)&serv, sizeof(serv));
	if (ret < 0)
	{
		perror("bind error");
	}

	//监听socket
	listen(lfd, 128);
	struct sockaddr_in client;
	int len = sizeof(client);
	//接受新连接，并获得通信文件描述符
	int cfd = accept(lfd, (struct sockaddr*)&client, &len);
	int i = 0;
	int n;
	char buf[1024];
	//读写数据
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		n = recv(cfd, buf, sizeof(buf), 0);
		if (n <= 0)
		{
			printf("read error");
			break;
		}
		printf("%s", buf);
		for (i = 0; i < n; i++)
		{
			buf[i] = toupper(buf[i]);
		}

		send(cfd, buf, n, 0);
	}

	closesocket(lfd);
	closesocket(cfd);
}

void client()
{
	//创建socket，获得监听文件描述符
	int cfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//TCP
	if (cfd == INVALID_SOCKET)
	{
		perror("socket error");
	}
	//链接服务端
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	//inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.S_un.S_addr);//WS2tcpip.h
	memcpy(&addr.sin_addr, "127.0.0.1", sizeof("127.0.0.1"));
	int ret = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
	if (ret < 0)
	{
		perror("connect error");
	}
	char buf[1024];
	while (1)
	{
		memset(buf, 0, sizeof(buf));//清空缓冲区
		//读标准输入数据
		int n = send(cfd, "hello", 5,0);
		if (n <= 0)
		{
			printf("read error");
			break;
		}
		//写数据
		recv(cfd, buf, sizeof(buf),0);
		printf("read=%s\n", buf);
	}
	closesocket(cfd);
}

int main(int argc, char* argv[])
{
	int ret;
#ifdef WIN32
	//配置windows socket版本
	WORD wVersionRequested;
	WSADATA WSAData;
	wVersionRequested = MAKEWORD(2, 2);
	ret = WSAStartup(wVersionRequested, &WSAData);
	if (ret != 0)
	{
		perror("WSAStartup error");
		return -1;
	}
#endif

	
	//service();
	client();

	//结束时清理定义的版本
#ifdef WIN32
	WSACleanup();
#endif

	system("pause");
}