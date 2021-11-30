#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>

void sighander(int signo)
{
	printf("signo=%d\n", signo);
}

int main(int argc, char* argv[])
{
	signal(SIGALRM, sighander);
	//������ʱ�丳ֵ
	struct itimerval tm;
	tm.it_interval.tv_sec = 1;
	tm.it_interval.tv_usec = 0;
	//��һ�δ���ʱ��
	tm.it_value.tv_sec = 3;
	tm.it_value.tv_usec = 0;

	setitimer(SIGALRM, &tm, NULL);

	while (1)
	{
		sleep(1);
	}
	return 0;
}