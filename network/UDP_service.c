#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include "wrap.h"

int main(int argc, char** argv)
{
	//创建socket
	int lfd = Socket(PF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr;
	addr.sin_port = htons(8888);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//绑定
	Bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
	char buf[1024];
	//客户端地址
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	while (1)
	{

		memset(buf, 0x00, sizeof(buf));
		//读
		int n = recvfrom(lfd, buf, sizeof(buf), 0, (struct sockaddr*)&clientAddr, &len);
		printf("%d:%s",ntohs(clientAddr.sin_port), buf);
		for (int i = 0; i < n; i++)
		{
			buf[i] = toupper(buf[i]);
		}
		//写
		sendto(lfd, buf, sizeof(buf), 0, (struct sockaddr*)&clientAddr, len);
	}

	Close(lfd);
	return 0;
}