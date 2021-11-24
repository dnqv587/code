#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc ,char* argv[])
{
	pid_t pid=fork();
	if(pid<0)
	{
		perror("fork error");
		return -1;
	}
	else if(pid>0)
	{
		printf("father:%d\n",getpid());
		sleep(1);
	}
	else if(pid==0)
	{
		printf("child:%d\n",getpid());
		sleep(1);
	}
	return 0;
}
