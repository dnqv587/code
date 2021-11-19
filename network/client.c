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
	//创建socket，获得监听文件描述符
	int kfd = socket(AF_INET, SOCK_STREAM, 0);
	if (kfd < 0)
	{
		perror("socket error");
		return -1;
	}
	//链接服务端
	struct sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);
	int ret = connect(kfd, (struct sockaddr *) &serv, sizeof(serv));
	if (ret < 0)
	{
		perror("connect error");
		return -1;
	}

	int n;
	char buf[256];
	while (1)
	{
		memset(buf, 0, sizeof(buf));//清空缓冲区
		//读标准输入数据
		n = read(STDIN_FILENO, buf, sizeof(buf));
		if (n <= 0)
		{
			printf("read error");
			break;
		}
		//写数据
		write(kfd, buf, sizeof(buf));
		printf("read=%s\n", buf);
	}
	close(kfd);
	return 0;
}