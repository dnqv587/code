#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <poll.h>

# include "wrap.h"
#define MAX_CLIENT_COUNT 1024 
int main()
{
	//创建socket
	int lfd= Socket(AF_INET, SOCK_STREAM, 0);
	//设置端口复用
	int reuse = 1;
	if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
		perr_exit("connect erro");
	//绑定IP，port
	struct sockaddr_in addr;
	//memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	//监听
	Listen(lfd, 128);
	//定义poll结构体
	struct pollfd client[MAX_CLIENT_COUNT];
	//for (int i = 0; i < MAX_CLIENT_COUNT; i++)
	//{
	//	client[i].fd = -1;
	//}
	memset(client, -1, sizeof(client));
	client[0].fd = lfd;
	client[0].events = POLLIN;
	//数组实际有效内容的个数/内核监听的范围
	int maxi = 0;
	//客户端地址
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);

	char buf[1024];
	char sIP[16];
	while (1)
	{
		int nready = poll(client, maxi + 1, -1);
		if (nready < 0)
		{
			if (errno == EINTR)
				continue;
			break;
		}
		//有客户端连接请求到来
		if (client[0].fd == lfd && (client[0].revents & POLLIN))
		{
			int cfd = Accept(lfd, (struct sockaddr*)&clientAddr, &len);
			memset(sIP, 0x00, sizeof(sIP));
			printf("client:%s %d\n", inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, sIP, sizeof(sIP)), ntohs(clientAddr.sin_port));
			for (int i = 0; i < MAX_CLIENT_COUNT; i++)
			{
				if (client[i].fd == -1)
				{
					client[i].fd = cfd;
					client[i].events = POLLIN;
					if (i > maxi)
						maxi = i;
					break;
				}
				if (i == MAX_CLIENT_COUNT)
				{
					close(cfd);
					break;
				}
			}
			if (--nready == 0)
				continue;
		}
		//客户端发送数据
		for (int i = 1; i <= maxi; i++)
		{
			int sockfd = client[i].fd;

			if (sockfd == -1)
				continue;
			if (client[i].revents & POLLIN)
			{
				memset(buf, 0x00, sizeof(buf));
				int n = read(sockfd, buf, sizeof(buf));
				if (n <= 0)
				{
					printf("read error or client closed,n==[%d]\n", n);
					close(sockfd);
					client[i].fd = -1;
					if (i == maxi)
						maxi--;
				}
				printf("client==%d,buf==%s\n", ntohs(clientAddr.sin_port), buf);

				for (int j = 0; j < n; j++)
				{
					buf[j] = toupper(buf[j]);
				}
				write(sockfd, buf, n);
				if (--nready == 0)
					break;
			}
			
		}
	}
	close(lfd);
	return 0;
}

