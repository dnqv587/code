#include <asm-generic/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MULTICAST_IP "239.255.0.1" //组播地址
#define MULTICAST_PORT 8888 //组播端口

int main()
{
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd <0)
    {
        perror("socket creation failed!");
        return -1;
    }

    //启用组播
    int multicast_enable = 1;
    int ret = setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_TTL,&multicast_enable,sizeof(multicast_enable));
    if(ret < 0)
    {
        perror("setsockopt failed!");
        close(sockfd);
        return -1;
    }

    struct sockaddr_in multicastAddr;
	multicastAddr.sin_port = htons(MULTICAST_PORT);
	multicastAddr.sin_family = AF_INET;
	inet_pton(AF_INET, MULTICAST_IP, (struct sockaddr*)&multicastAddr.sin_addr.s_addr);

    const char msg[] = "Multicast:Hello!";

    int sendRes = sendto(sockfd,msg,strlen(msg),0,(struct sockaddr*)&multicastAddr,sizeof(multicastAddr));
    if(sendRes < 0)
    {
        perror("multicast message failed");
    }
    else
    {
        printf("multicast message:%s\n",msg);
    }

    close(sockfd);

    return 0;
}
