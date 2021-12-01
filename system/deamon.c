#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/stat.h>



int TCP()
{
	//创建socket，获得监听文件描述符
	int kfd = socket(AF_INET, SOCK_STREAM, 0);
	if (kfd < 0)
	{
		perror("socket error");
		return -1;
	}

	struct sockaddr_in serv;
	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	//绑定socket
	int ret = bind(kfd, (struct sockaddr*)&serv, sizeof(serv));
	if (ret < 0)
	{
		perror("bind error");
		return -1;
	}
	//监听socket
	listen(kfd, 128);
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	//接受新连接，并获得通信文件描述符
	int cfd = accept(kfd, (struct sockaddr*)&client, &len);
	int i = 0;
	int n = 0;
	char buf[1024];
	//读写数据
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		n = read(cfd, buf, sizeof(buf));
		if (n <= 0)
		{
			printf("read error");
			break;
		}
		printf("%s", buf);
		for (i = 0; i < n; i++)
		{
			buf[i] = toupper(buf[i]);
		}

		write(cfd, buf, n);
	}

	close(kfd);
	close(cfd);


}


int main(int argc, char* argv[])
{
	//创建子进程，父进程退出
	pid_t pid = fork();
	if (pid < 0 || pid>0)
	{
		exit(1);
	}
	//创建会话
	pid_t sid = setsid();
	//改变当前工作目录
	chdir("/home/dai/projects/code/system");
	//改变文件掩码
	umask(0000);
	//关闭标准输入、标准输出、标准错误
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	TCP();
}