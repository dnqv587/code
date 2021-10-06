#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include "wrap.h"

int main()
{
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv;
	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = Bind(lfd, (struct sockaddr*)&serv, sizeof(serv));
	Listen(lfd, 128);

	int cfd;
	pid_t pid;

	struct sockaddr_in client;
	socklen_t len ;

	char sIP[16];
	while (1)
	{
		len = sizeof(client);
		memset(sIP, 0, sizeof(sIP));
		cfd = Accept(lfd, (struct sockaddr*)&client, &len);
		printf("client:[%s] [%d]\n", inet_ntop(AF_INET, &client.sin_addr.s_addr, sIP, sizeof(sIP)), ntohs(client.sin_port));

		pid = fork();
		if (pid < 0)
		{
			perror("fork error");
			exit(-1);
		}
		else if (pid > 0)
		{
			close(cfd);
		}
		else if (pid == 0)
		{
			close(lfd);
			int n;
			char buf[1024];
			
			while (1)
			{
				memset(buf, 0, sizeof(buf));
				n = Read(cfd, buf, sizeof(buf));
				if (n <= 0)
				{
					printf("read error");
					break;
				}
				printf("prot=[%d],n=[%d],read=[%s]\n", ntohs(client.sin_port),n, buf);
				for (int i = 0; i < n; i++)
				{
					buf[i] = toupper(buf[i]);
				}
				Write(cfd, buf, n);

			}
			
			close(cfd);
			exit(0);

		}
	}

	close(lfd);
	return 0;
}