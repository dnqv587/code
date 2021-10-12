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
	//����socket
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	//���ö˿ڸ���
	int reuse = 1;
	if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
		perr_exit("connect erro");
	//��IP��port
	struct sockaddr_in addr;
	//memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
	//����
	Listen(lfd, 128);
	//����poll�ṹ��
	struct pollfd client[MAX_CLIENT_COUNT];
	for (int i = 0; i < MAX_CLIENT_COUNT; i++)
	{
		client[i].fd = -1;
	}
	//memset(client, -1, sizeof(client));
	client[0].fd = lfd;
	client[0].events = POLLIN;
	//����ʵ����Ч���ݵĸ���/�ں˼����ķ�Χ
	int maxi = 0;

	char buf[1024];
	
	while (1)
	{
		int nready = poll(client, maxi + 1, -1);
		if (nready < 0)
		{
			if (errno == EINTR)
				continue;
			break;
		}
		//�пͻ�������������
		if (client[0].revents & POLLIN)
		{
			int cfd = Accept(lfd, NULL, NULL);
			printf("%d", cfd);
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
				if (i = MAX_CLIENT_COUNT)
				{
					close(cfd);
					break;
				}
			}
			if (--nready == 0)
				continue;
		}
		//�ͻ��˷�������
		for (int i = 1; i <= maxi; i++)
		{
			int sockfd = client[i].fd;
			memset(buf, 0x00, sizeof(buf));
			if (sockfd == -1)
				continue;
			if (client[i].revents & POLLIN)
			{
				int n = read(sockfd, buf, sizeof(buf));
				if (n < 0)
				{
					printf("read error or client closed,n==[%d]\n", n);
					close(sockfd);
					client[i].fd = -1;
					if (i == maxi)
						maxi--;
				}
				for (int j = 0; j < n; j++)
				{
					buf[j] = toupper(buf[j]);
				}
				printf("n==[%d],buf==[%s]\n", n, buf);
				write(sockfd, buf, n);
				if (--nready == 0)
					break;
			}
			
		}
	}
	close(lfd);
	return 0;
}