
#include "wrap.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <event2/event.h>
#include <ctype.h>

//typedef void (*event_callback_fn)(evutil_socket_t fd, short events, void* arg);
struct event* connectevent = NULL;
//数组分别存储每个通信文件描述符对应的事件
struct event* cfdEvent[1024];
//通信文件描述符对应的回调函数
void readCallback(evutil_socket_t fd, short events, void* arg)
{
	char buf[1024];
	memset(buf, 0x00, sizeof(buf));
	//读数据
	int n = read(fd, buf, sizeof(buf));
	if (n <= 0)
	{
		//从base地基上删除该事件
		close(fd);
		event_del(cfdEvent[fd]);
		event_free(cfdEvent[fd]);
	}
	printf("%s", buf);
	for (int i = 0; i < n; i++)
	{
		buf[i] = toupper(buf[i]);
	}
	//发送数据给对方
	write(fd, buf, n);
}
//监听文件描述符对应的回调函数
void connectCallback(evutil_socket_t fd, short events, void* arg)
{

	int cfd = Accept(fd, NULL, NULL);
	if (cfd > 0)
	{
		//创建通信文件描述符对应的事件
		connectevent = event_new((struct event_base*)arg, cfd, EV_READ | EV_PERSIST, readCallback, NULL);
		cfdEvent[cfd] = connectevent;
		if (connectevent == NULL)
		{
			//退出循环
			event_base_loopexit((struct event_base*)arg, NULL);
		}
		event_add(connectevent, NULL);
	}
}

int main(int argc,char** argv)
{
	//创建socket
	int lfd = Socket(PF_INET, SOCK_STREAM, 0);
	//设置端口复用
	int reuse = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	//绑定
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8888);
	socklen_t len = sizeof(addr);
	Bind(lfd, (struct sockaddr*)&addr, len);
	//监听
	Listen(lfd, 1024);
	//创建event地基
	struct event_base* base=event_base_new();
	if (base == NULL)
	{
		printf("event_base_new error\n");
		return -1;
	}
	//创建event
	struct event* event = event_new(base, lfd, EV_READ | EV_PERSIST, connectCallback, base);
	if (event == NULL)
	{
		printf("event_new error\n");
		return -1;
	}
	//上event_base
	event_add(event,NULL);
	//进入事件循环
	event_base_dispatch(base);
	// 释放资源
	event_base_free(base);
	event_free(event);
	return 0;
}