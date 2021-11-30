#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	//创建管道  
	int pipe_fd[2];
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe error");
		return -1;
	}
	//创建子进程 
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		return -1;
	}
	else if (pid > 0)
	{
		close(pipe_fd[0]);
		//将标准输出重定向为管道写端
		if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 error");
		}
		execlp("ps", "ps", "aux", NULL);
		perror("execlp error");
		//wait(NULL);
	}
	else if (pid == 0)
	{
		close(pipe_fd[1]);
		//将标准输入重定向为管道读端
		if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		{
			perror("dup2 error");
		}
		execlp("grep", "grep", "--color=auto", "bash", NULL);
		perror("execlp error");
	}
	return 0;
}
