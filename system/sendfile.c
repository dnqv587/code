#include <sys/sendfile.h>
#include <fcntl.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>

int kfd;

int server(){
		//创建socket，获得监听文件描述符
	kfd=socket(AF_INET,SOCK_STREAM,0);
	if (kfd<0)
	{
		perror("socket error");
		return -1;
	}

	struct sockaddr_in serv;
	bzero(&serv,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(4567);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	//绑定socket
	int ret=bind(kfd,(struct sockaddr *)&serv,sizeof(serv));
	if (ret<0)
	{
		perror("bind error");
		return -1;
	}
	//监听socket
	listen(kfd,128);
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	//接受新连接，并获得通信文件描述符
	int cfd = accept(kfd, (struct sockaddr *) &client, &len);
	return cfd;
}

int main(){
	int file_fd = open("sendfile.c", O_RDONLY);
	int socket_fd = server();
	off_t offset = 0;
	size_t file_size = lseek(file_fd, 0, SEEK_END);

	sendfile(socket_fd, file_fd, &offset, file_size);

	getchar();
	close(file_fd);
	close(socket_fd);
	close(kfd);
	
	return 0;
}
