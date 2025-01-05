#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define MULTICAST_PORT 8888
#define MULTICAST_IP "239.255.0.1"

int main(int argc, char** argv)
{
    //创建socket
    int recvfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(recvfd < 0)
    {
        perror("socket creation failed!");
        return -1;
    }
    //绑定到组播端口
    struct sockaddr_in addr;
    addr.sin_port = htons(8888);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定
    int bindret = bind(recvfd, (struct sockaddr*)&addr, sizeof(addr));
    if(bindret < 0)
    {
        perror("binding failed!");
        close(recvfd);
        return -1;
    }
    //加入组播组
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    int optret = setsockopt(recvfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&multicastRequest,sizeof(multicastRequest));
    if(optret < 0)
    {
        perror("join multicast group failed!");
        close(recvfd);
        return -1;
    }

    //客户端地址
    struct sockaddr_in sendAddr;
    socklen_t len = sizeof(sendAddr);

    char buf[1024];
    memset(buf, 0x00, sizeof(buf));
    //接受组播消息
    int n = recvfrom(recvfd, buf, sizeof(buf), 0, (struct sockaddr*)&sendAddr, &len);
    if(n > 0)
    {
        buf[n] = 0;
        printf("%d:%s",ntohs(sendAddr.sin_port), buf);
    }
    else
    {
        perror("recv failed!");
    }

    //退出组播组
    setsockopt(recvfd,IPPROTO_IP,IP_DROP_MEMBERSHIP,(char*)&multicastRequest,sizeof(multicastRequest));

    close(recvfd);
    return 0;
}
