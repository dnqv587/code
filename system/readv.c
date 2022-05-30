#include <sys/uio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
 
int main()
{
    struct iovec buf[3];
    char n='\n';
    int rn=readv(STDIN_FILENO,buf,3);
    int len= writev(STDOUT_FILENO,buf,3);
    printf("read:%d  written:%d\n",rn,len);
    return 0;
}

