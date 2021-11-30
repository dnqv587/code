#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	//�����ܵ�
	int fd[2];
	if (pipe(fd) < 0)
	{
		perror("pipe error");
		return -1;
	}

	char buf[1024];
	//�����ӽ���
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		return -1;
	}
	else if (pid > 0)
	{
		//�رն���
		close(fd[0]);

		while (1)
		{
			memset(buf, 0x00, sizeof(buf));
			read(STDIN_FILENO, buf, sizeof(buf));
			write(fd[1], buf, strlen(buf));
		}
		//wait(NULL);
	}
	else
	{
		//�ر�д��
		close(fd[1]);
		while (1)
		{
			memset(buf, 0x00, sizeof(buf));
			read(fd[0], buf, sizeof(buf));
			printf("read:%s", buf);
		}
	}

	return 0;
}
