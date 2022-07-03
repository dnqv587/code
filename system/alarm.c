#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sig_handler()
{
    printf("sig_handler\n");
}

int main(int argc,char* argv)
{
    //注册SIGALRM的handler
    signal(SIGALRM,sig_handler);
    alarm(5);
    for(int i=0;i<11;++i)
    {
        printf("sleep %d ...\n",i);
        sleep(1);
    }
    return 0;
}
