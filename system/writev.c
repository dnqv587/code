#include <sys/uio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    struct iovec buf[3];
    char hello[]="hello ";
    char n='\n';
    char world[]="world!";
    buf[0].iov_base=hello;
    buf[0].iov_len=strlen(hello);
    buf[1].iov_base=world;
    buf[1].iov_len=strlen(world);
    buf[2].iov_base=&n;
    buf[2].iov_len=1;
    int len= writev(STDOUT_FILENO,buf,3);
    printf("written:%d\n",len);
    return 0;
}
