#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <pthread.h>
#include "wrap.h"

void* thread_work(void* arg)
{
	int cfd = *(int*)arg;

	int n;
	char buf[1024];

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		n = Read(cfd, buf, sizeof(buf));
		if (n <= 0)
		{
			printf("read error");
			pthread_exit(NULL);
		}
		printf("n=[%d],read=[%s]\n",  n, buf);
		for (int i = 0; i < n; i++)
		{
			buf[i] = toupper(buf[i]);
		}
		Write(cfd, buf, n);
	}
	close(cfd);
	pthread_exit(NULL);
}

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

	struct sockaddr_in client;
	socklen_t len;
	int cfd;
	pthread_t threadID;
	char sIP[16];
	while (1)
	{
		cfd = Accept(lfd, (struct sockaddr*)&client, &len);
		printf("client:[%s] [%d]\n", inet_ntop(AF_INET, &client.sin_addr.s_addr, sIP, sizeof(sIP)), ntohs(client.sin_port));

		pthread_create(&threadID, NULL, thread_work, &cfd);
		pthread_detach(threadID);
	}

	close(lfd);
	return 0;
}
