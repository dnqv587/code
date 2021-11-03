
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

//�¼������ṹ��
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

int epfd = 0;//�����ڵ�

char sIP[16];
struct sockaddr_in clientAddr;
socklen_t len = sizeof(clientAddr);

xevent myevents[_EVENT_SIZE_+1];

void readData(int fd, int events, void* arg);

//����¼�
void eventAdd(int fd, int events, void (*call_back)(int, int, void*), void* arg, xevent* ev)
{
	//д���¼�����
	ev->fd = fd;
	ev->events = events;
	ev->call_back = call_back;
	ev->arg = arg;
	//��lfd��������ӻص�����
	struct epoll_event epv;
	epv.events = events;
	epv.data.ptr = ev;//��epv��ָ��ָ���¼�����ev
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &epv);
}

//�޸��¼�
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

//ɾ���¼�
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

//д�¼�����
void sendData(int fd, int events, void* arg)
{
	xevent* ev = arg;
	for (int i = 0; i < ev->buflen; i++)
	{
		ev->buf[i] = toupper(ev->buf[i]);
	}
	Write(fd, ev->buf, ev->buflen);
	printf("%d->send:%s", clientAddr.sin_port,ev->buf);
	eventModify(fd, EPOLLIN, readData, ev);
}
//���¼�����
void readData(int fd, int events, void *arg)
{
	xevent* ev = arg;
	ev->buflen = Read(fd, ev->buf, sizeof(ev->buf));
	//��������
	if (ev->buflen > 0)
	{
		printf("%d->read:%s",clientAddr.sin_port, ev->buf);
		//�л��¼�Ϊд�¼�
		eventModify(fd, EPOLLOUT, sendData, ev);
	}
	//�Է��ر�����
	if (ev->buflen == 0)
	{
		printf("client closed");
		Close(fd);
		//ɾ���¼�
		eventDelete(fd, EPOLLIN, ev);
	}
}

//�����Ӵ���
void initAccept(int fd, int events, void* arg)
{

	int cfd = Accept(fd, (struct sockaddr*)&clientAddr, &len);
	//printf("client:%s %d", inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, sIP, sizeof(sIP)), ntohs(clientAddr.sin_port));
	for (int i = 0; i < _EVENT_SIZE_; i++)
	{
		if (myevents[i].fd == 0)
		{
			//���ö��¼�
			eventAdd(cfd, EPOLLIN, readData, &myevents[i], &myevents[i]);
			break;
		}
	}
}

int main(int argc,char **argv)
{
	//����socket
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	//�˿ڸ���
	int reuse = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	//�󶨶˿�
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
	//����
	Listen(lfd, 1024);
	//����epoll���ڵ�
	epfd = epoll_create(1024);
	//�洢epoll_wait��������
	struct epoll_event events[_EVENT_SIZE_];
	//������ʼ�¼���������������������

	eventAdd(lfd, EPOLLIN, initAccept, &myevents[_EVENT_SIZE_], &myevents[_EVENT_SIZE_]);

	while (1)
	{
		int nready = epoll_wait(epfd, events, _EVENT_SIZE_, -1);

		if (nready < 0)
		{
			if (errno == EINTR)
				continue;
			break;
		}
		if (nready > 0)//����epoll_wait�ɹ�,�������¼��������ļ��������ĸ���
		{
			for (int i = 0; i < nready; i++)
			{
				xevent* xe = events[i].data.ptr;//ȡptrָ��ṹ���ַ
				if (xe->events == events[i].events)
				{
					xe->call_back(xe->fd, xe->events, xe);//�����¼���Ӧ�Ļص�
				}
			}
		}
	}
	Close(lfd);
	return 0;
}