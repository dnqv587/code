
#include "wrap.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <event2/event.h>
#include <ctype.h>

//typedef void (*event_callback_fn)(evutil_socket_t fd, short events, void* arg);
struct event* connectevent = NULL;
//����ֱ�洢ÿ��ͨ���ļ���������Ӧ���¼�
struct event* cfdEvent[1024];
//ͨ���ļ���������Ӧ�Ļص�����
void readCallback(evutil_socket_t fd, short events, void* arg)
{
	char buf[1024];
	memset(buf, 0x00, sizeof(buf));
	//������
	int n = read(fd, buf, sizeof(buf));
	if (n <= 0)
	{
		//��base�ػ���ɾ�����¼�
		close(fd);
		event_del(cfdEvent[fd]);
		event_free(cfdEvent[fd]);
	}
	printf("%s", buf);
	for (int i = 0; i < n; i++)
	{
		buf[i] = toupper(buf[i]);
	}
	//�������ݸ��Է�
	write(fd, buf, n);
}
//�����ļ���������Ӧ�Ļص�����
void connectCallback(evutil_socket_t fd, short events, void* arg)
{

	int cfd = Accept(fd, NULL, NULL);
	if (cfd > 0)
	{
		//����ͨ���ļ���������Ӧ���¼�
		connectevent = event_new((struct event_base*)arg, cfd, EV_READ | EV_PERSIST, readCallback, NULL);
		cfdEvent[cfd] = connectevent;
		if (connectevent == NULL)
		{
			//�˳�ѭ��
			event_base_loopexit((struct event_base*)arg, NULL);
		}
		event_add(connectevent, NULL);
	}
}

int main(int argc,char** argv)
{
	//����socket
	int lfd = Socket(PF_INET, SOCK_STREAM, 0);
	//���ö˿ڸ���
	int reuse = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	//��
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8888);
	socklen_t len = sizeof(addr);
	Bind(lfd, (struct sockaddr*)&addr, len);
	//����
	Listen(lfd, 1024);
	//����event�ػ�
	struct event_base* base=event_base_new();
	if (base == NULL)
	{
		printf("event_base_new error\n");
		return -1;
	}
	//����event
	struct event* event = event_new(base, lfd, EV_READ | EV_PERSIST, connectCallback, base);
	if (event == NULL)
	{
		printf("event_new error\n");
		return -1;
	}
	//��event_base
	event_add(event,NULL);
	//�����¼�ѭ��
	event_base_dispatch(base);
	// �ͷ���Դ
	event_base_free(base);
	event_free(event);
	return 0;
}