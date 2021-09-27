#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
int main()
{
	int kfd=socket(AF_INET,SOCK_STREAM,0);
	if (kfd<0)
	{
		printf("create socket error");
		return -1;
	}

	struct sockaddr_in serv;
	bzero(&serv,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(8888);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);

	int ret=bind(kfd,(struct sockaddr *)&serv,sizeof(serv));
	if (ret<0)
	{
		perror("bind error");
		return -1;
	}
	
	listen(kfd,128);

	int cfd = accept (kfd,NULL,NULL);
	int i=0;
	int n=0;
	char buf[1024];

	while (1)
	{
		memset(buf,0,sizeof(buf));
		n=read(cfd,buf,sizeof(buf));
		printf("%s",buf);
	//	for(i=0;i<n;i++)
	//	{
	//		buf[i]=toupper(buf[i]);
	//	}

		write(cfd,buf,n);
	}
	
	close (kfd);
	close (cfd);


}
