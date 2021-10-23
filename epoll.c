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
	//����socket
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	//���ö˿ڸ���
	int reuse = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	//��ip��port
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
	//����
	Listen(lfd, 1024);
	//����epoll��
	int epfd = epoll_create(1024);
	if (epfd < 0)
	{
		perror("create epoll fail");
		return -1;
	}
	//�������ļ���������epoll��
	struct epoll_event ev;
	ev.events = EPOLLIN;//�������¼�
	ev.data.fd = lfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	//�ȴ��ں˷����¼�����
	struct epoll_event events[1024];
	//������
	char buf[1024];
	//�ͻ�����Ϣ
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
			//�пͻ�����������
			if (fd == lfd)
			{
				int cfd = Accept(lfd, (struct sockaddr *)&clientAddr, &len);
				//printf("client:%s,%d", inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, sIP, sizeof(sIP)),ntohs(clientAddr.sin_port));
				//��cfd��Ӧ�Ķ��¼��϶�poll��
				ev.data.fd = cfd;
				ev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
				continue;
			}
			//�пͻ��˷�������
			memset(buf, 0x00, sizeof(buf));
			int n = Read(fd, buf, sizeof(buf));
			printf("%d:buf=%s", ntohs(clientAddr.sin_port),buf);
			if (n <= 0)
			{
				close(fd);
				//��fd��Ӧ���¼��ڵ��epoll����ɾ��
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