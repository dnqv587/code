#include <asm-generic/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BROADCAST_IP "255.255.255.255" //广播地址
#define BROADCAST_PORT 8888 //发送广播的端口

int main()
{
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd <0)
    {
        perror("socket creation failed!");
        return -1;
    }

    //启用广播
    int broadcast_enable = 1;
    int ret = setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&broadcast_enable,sizeof(broadcast_enable));
    if(ret < 0)
    {
        perror("setsockopt failed!");
        close(sockfd);
        return -1;
    }

    struct sockaddr_in broadcastAddr;
	broadcastAddr.sin_port = htons(BROADCAST_PORT);
	broadcastAddr.sin_family = AF_INET;
	inet_pton(AF_INET, BROADCAST_IP, (struct sockaddr*)&broadcastAddr.sin_addr.s_addr);

    const char msg[] = "Broadcast:Hello!";

    int sendRes = sendto(sockfd,msg,strlen(msg),0,(struct sockaddr*)&broadcastAddr,sizeof(broadcastAddr));
    if(sendRes < 0)
    {
        perror("broadcast message failed");
    }
    else
    {
        printf("broadcast message:%s\n",msg);
    }

    close(sockfd);

    return 0;
}
