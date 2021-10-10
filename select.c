#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>

# include "wrap.h"
int main()
{
	//创建socket
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	//端口复用
	int reuse = 1;
	if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		perr_exit("connect error");
	//绑定端口
	struct sockaddr_in serv;
	memset(&serv, 0x00, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&serv, sizeof(serv));
	//监听
	Listen(lfd, 128);
	//select
	fd_set readfds;//定义文件描述符集
	fd_set tmpfds;//临时文件描述符集
	FD_ZERO(&readfds);//清空文件描述符集
	FD_ZERO(&tmpfds); 
	FD_SET(lfd, &readfds);//将lfd加入到readfds集合中去
	int maxfd = lfd;//最大文件描述符
	int nready;
	//读写
	int n = 0;
	char buf[1024];

	//代码优化
	int clientFD[1024];//用来存储通信描述符
	memset(clientFD, -1, sizeof(clientFD));
	int maxi=1;//存储clientFD最大有效下标
	while (1)
	{
		tmpfds = readfds;
		nready = select(maxfd + 1, &tmpfds, NULL, NULL, NULL);
		if (nready < 0)
		{
			if (errno == EINTR)//被信号中断
				continue;
			break;
		}
		//printf("nready=%d\n", nready);
		//有客户端请求
		if (FD_ISSET(lfd, &tmpfds))
		{
			//接受新的客户端请求
			int cfd = Accept(lfd, NULL, NULL);
			//将cfd加入到文件描述符集中去
			FD_SET(cfd, &readfds);
			//数组存入有效通信文件描述符
			clientFD[maxi++] = cfd;
			//修改内核监控的文件描述符的范围
			if (maxfd < cfd)
				maxfd = cfd;
			if (--nready == 0)
				continue;
		}
		printf("maxi=%d\n", maxi);
		//有客户端数据发来
		//for (int i = lfd + 1; i <= maxfd ; i++)
		for (int i = 1; i < maxi; i++)
		{
			printf("clientFD[%d]=%d\n", i, clientFD[i]);
		
			if (clientFD[i] > 0)
			{
				if (FD_ISSET(clientFD[i], &tmpfds))
				{
					while (1)
					{
						memset(buf, 0x00, sizeof(buf));
						//读数据
						n = read(clientFD[i], buf, sizeof(buf));
						if (n <= 0)
						{
							close(clientFD[i]);
							FD_CLR(i, &readfds);//将文件描述符--sockfd从内核中去除
							clientFD[i] = -1;//将无效的通信文件描述符从数组中去除
							printf("read error or connect are closed");
						}

						printf("%s", buf);
						for (int j = 0; j < n; j++)
						{
							buf[j] = toupper(buf[j]);
						}
						//应答数据给客户端
						write(clientFD[i], buf, n);

						if (--nready == 0)
							break;

					}
				}
			}
		}
	}	
	close(lfd);
	return 0;
}