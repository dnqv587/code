#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <wait.h>

//SIGCHLD信号处理函数
void sighandler(int signo)
{
	//printf("signo:%d\n", signo);
	while (1)
	{
		pid_t wpid = waitpid(-1, NULL, WNOHANG);
		if (wpid > 0)
		{
			printf("wpid:%d\n", wpid);
		}
		else if (wpid == 0)
		{
			printf("living");
			break;
		}
		else if (wpid == -1)
		{
			printf("no child is living");
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	//将SIGCHLD信号阻塞
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &mask, NULL);
	//创建子线程
	int i;
	for (  i = 0; i < 3; ++i)
	{
		pid_t pid = fork();
		if (pid < 0)
		{
			perror("fork error");
			return -1;
		}
		else if (pid > 0)
		{
			
			
		}
		else if (pid == 0)
		{
			break;
		}
	}

	if (i == 0)
	{
		printf("child_pid:%d\n", getpid());
		//sleep(1);
	}
	if (i == 1)
	{
		printf("child_pid:%d\n", getpid());
		//sleep(2);
	}

	if (i == 2)
	{
		printf("child_pid:%d\n", getpid());
		//sleep(3);
	}
	if (i == 3)
	{
		//注册对SIGCHLD信号处理函数
		sleep(5);
		struct sigaction act;
		act.sa_handler = &sighandler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD, &act, NULL);
		//完成对SIGCHLD信号注册后，解除对SIGCHLD信号的阻塞
		sigprocmask(SIG_UNBLOCK, &mask, NULL);

		printf("parent_pid:%d\n", getpid());
		while (1)
		{
			sleep(1);
		}
	}
}