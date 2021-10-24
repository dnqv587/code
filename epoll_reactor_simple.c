
#include "wrap.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <ctype.h>

#define _BUF_LEN_ 1024
#define _EVENT_SIZE_ 1024

//事件驱动结构体
typedef struct xx_event
{
	int fd;
	int events;
	void (*call_back)(int fd, int events,void * arg);
	void* arg;
	char buf[_BUF_LEN_];
	int buflen;
	int epfd;
}xevent;

int epfd = 0;//树根节点

xevent myevents[_EVENT_SIZE_+1];

void readData(int fd, int events, void* arg);

//添加事件
void eventAdd(int fd, int events, void (*call_back)(int, int, void*), void* arg, xevent* ev)
{
	//写入事件驱动
	ev->fd = fd;
	ev->events = events;
	ev->call_back = call_back;
	ev->arg = arg;
	//将lfd上树并添加回调函数
	struct epoll_event epv;
	epv.events = events;
	epv.data.ptr = ev;//将epv的指针指向事件驱动ev
	epoll_ctl(fd, EPOLL_CTL_ADD, fd, &epv);
}

//修改事件
void eventModify(int fd, int events, void (*call_back)(int, int, void*),xevent* ev)
{
	ev->fd = fd;
	ev->events = events;
	ev->call_back = call_back;

	struct epoll_event epv;
	epv.events = events;
	epv.data.ptr = ev;
	epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &epv);
}

//删除事件
void eventDelete(int fd, int events, xevent *ev)
{
	ev->fd = 0;
	ev->events = 0;
	ev->call_back = NULL;
	memset(ev->buf, 0x00, sizeof(ev->buf));
	ev->buflen = 0;

	struct epoll_event epv;
	epv.events = events;
	epv.data.ptr = NULL;
	epoll_ctl(epfd,EPOLL_CTL_DEL, fd, &epv);
}

//写事件处理
void sendData(int fd, int events, void* arg)
{
	xevent* ev = arg;
	for (int i = 0; i < ev->buflen; i++)
	{
		ev->buf[i] = toupper(ev->buf[i]);
	}
	Write(fd, ev->buf, ev->buflen);
	eventModify(fd, EPOLLIN, readData, ev);
}
//读事件处理
void readData(int fd, int events, void *arg)
{
	xevent* ev = arg;
	ev->buflen = Read(fd, ev->buf, sizeof(ev->buf));
	//读到数据
	if (ev->buflen > 0)
	{
		printf("%s", ev->buf);
		//切换事件为写事件
		eventModify(fd, EPOLLOUT, sendData, ev);
	}
	//对方关闭链接
	if (ev->buflen == 0)
	{
		printf("client closed");
		Close(fd);
		//删除事件
		eventDelete(fd, EPOLLIN, ev);
	}
}

//新连接处理
void initAccept(int fd, int events, void* arg)
{

	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int cfd = Accept(fd, (struct sockaddr*)&addr, &len);
	for (int i = 0; i < _EVENT_SIZE_; i++)
	{
		if (myevents[i].fd == 0)
		{
			//设置读事件
			eventAdd(cfd, EPOLLIN, readData, &myevents[i], &myevents[i]);
			break;
		}
	}
}

int main(int argc,char **argv)
{
	//创建socket
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	//端口复用
	int reuse = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	//绑定端口
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
	//监听
	Listen(lfd, 1024);
	//创建epoll根节点
	epfd = epoll_create(1024);
	//存储epoll_wait传出参数
	struct epoll_event events[_EVENT_SIZE_];
	//添加最初始事件，将侦听的描述符上树
	eventAdd(lfd, EPOLLIN, initAccept, &myevents[_EVENT_SIZE_], &myevents[_EVENT_SIZE_]);

	while (1)
	{
		printf("1");
		int nready = epoll_wait(epfd, events, _EVENT_SIZE_, -1);

		if (nready < 0)
		{
			if (errno == EINTR)
				continue;
			break;
		}
		if (nready > 0)//调用epoll_wait成功,返回有事件发生的文件描述符的个数
		{
			for (int i = 0; i < nready; i++)
			{
				xevent* xe = events[i].data.ptr;//取ptr指向结构体地址
				if (xe->events == events[i].events)
				{
					xe->call_back(xe->fd, xe->events, xe);//调用事件对应的回调
				}
			}
		}
	}
	Close(lfd);
	return 0;
}