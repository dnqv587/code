#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/epoll.h>
# include "wrap.h"

int main()
{
	//创建socket
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	//设置端口复用
	int reuse = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	//绑定ip、port
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
	//监听
	Listen(lfd, 1024);
	//创建epoll树
	int epfd = epoll_create(1024);
	if (epfd < 0)
	{
		perror("create epoll fail");
		return -1;
	}
	//将监听文件描述符上epoll树
	struct epoll_event ev;
	ev.events = EPOLLIN;//监听读事件
	ev.data.fd = lfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	//等待内核返回事件发生
	struct epoll_event events[1024];
	//缓冲区
	char buf[1024];
	//客户端信息
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	char sIP[16];

	while (1)
	{

		int nready = epoll_wait(epfd, events, 1024, -1);
		if (nready < 0)
		{
			if (errno == EINTR)
				continue;
			break;
		}
		for (int i = 0; i < nready; i++)
		{
			int fd = events[i].data.fd;
			//有客户端链接请求
			if (fd == lfd)
			{
				int cfd = Accept(lfd, (struct sockaddr *)&clientAddr, &len);
				//printf("client:%s,%d", inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, sIP, sizeof(sIP)),ntohs(clientAddr.sin_port));
				//将cfd对应的读事件上二poll树
				ev.data.fd = cfd;
				ev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
				continue;
			}
			//有客户端发送数据
			memset(buf, 0x00, sizeof(buf));
			int n = Read(fd, buf, sizeof(buf));
			printf("%d:buf=%s", ntohs(clientAddr.sin_port),buf);
			if (n <= 0)
			{
				close(fd);
				//将fd对应的事件节点从epoll树上删除
				epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
				perror("read error or client closed\n");
				continue;
			}
			for (int i = 0; i < n; i++)
				buf[i] = toupper(buf[i]);
			Write(fd, buf, sizeof(buf));
		}
	}
	close(epfd);
	close(lfd);
	return 0;
}