#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include<arpa/inet.h>
#include "wrap.h"

int main(int argc, char** argv)
{
	//����socket
	int lfd = Socket(PF_INET, SOCK_DGRAM, 0);

	char buf[1024];
	//�ͻ��˵�ַ
	struct sockaddr_in serviceAddr;
	serviceAddr.sin_port = htons(8888);
	serviceAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", (struct sockaddr*)&serviceAddr.sin_addr.s_addr);
	socklen_t len = sizeof(serviceAddr);
	while (1)
	{
		memset(buf, 0x00, sizeof(buf));
		//����׼��������
		Read(STDIN_FILENO, buf, sizeof(buf));
		//д
		sendto(lfd, buf, sizeof(buf), 0, (struct sockaddr*)&serviceAddr, len);
		//��
		memset(buf, 0x00, sizeof(buf));
		int n = recvfrom(lfd, buf, sizeof(buf), 0, NULL, NULL);
		printf("%d:%s", ntohs(serviceAddr.sin_port), buf);

	}

	Close(lfd);
	return 0;
}