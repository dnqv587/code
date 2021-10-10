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
	//����socket
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	//�˿ڸ���
	int reuse = 1;
	if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		perr_exit("connect error");
	//�󶨶˿�
	struct sockaddr_in serv;
	memset(&serv, 0x00, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&serv, sizeof(serv));
	//����
	Listen(lfd, 128);
	//select
	fd_set readfds;//�����ļ���������
	fd_set tmpfds;//��ʱ�ļ���������
	FD_ZERO(&readfds);//����ļ���������
	FD_ZERO(&tmpfds); 
	FD_SET(lfd, &readfds);//��lfd���뵽readfds������ȥ
	int maxfd = lfd;//����ļ�������
	int nready;
	//��д
	int n = 0;
	char buf[1024];
	int sockfd;
	while (1)
	{
		tmpfds = readfds;
		nready = select(maxfd + 1, &tmpfds, NULL, NULL, NULL);
		if (nready < 0)
		{
			if (errno == EINTR)//���ź��ж�
				continue;
			break;
		}
		printf("nready=%d\n", nready);
		//�пͻ�������
		if (FD_ISSET(lfd, &tmpfds))
		{
			//�����µĿͻ�������
			int cfd = Accept(lfd, NULL, NULL);
			//��cfd���뵽�ļ�����������ȥ
			FD_SET(cfd, &readfds);
			//�޸��ں˼�ص��ļ��������ķ�Χ
			if (maxfd < cfd)
				maxfd = cfd;
			if (--nready == 0)
				continue;
		}
		//�пͻ������ݷ���
		for (int i = lfd + 1; i <= maxfd ; i++)
		{
			sockfd = i;
			if (FD_ISSET(sockfd, &tmpfds))
			{
				while (1)
				{
					memset(buf, 0x00, sizeof(buf));
					//������
					n = read(sockfd, buf, sizeof(buf));
					if (n <= 0)
					{
						close(sockfd);
						FD_CLR(i, &readfds);//���ļ�������--sockfd���ں���ȥ��
						printf("read error or connect are closed");
					}

					printf("%s", buf);
					for (int j = 0; j < n; j++)
					{
						buf[j] = toupper(buf[j]);
					}
					//Ӧ�����ݸ��ͻ���
					write(sockfd, buf, n);

					if (--nready == 0)
						break;

				}
			}
		}
	}	
	close(lfd);
	return 0;
}