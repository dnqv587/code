#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	//打开文件
	int file_fd = open("./IO/text.txt", O_RDWR);
	if (file_fd < 0)
	{
		perror("open error");
		return -1;
	}

	//获取文件长度
	struct stat* file_buf;
	if (stat("./IO/text.txt", file_buf));
	int file_len = file_buf->st_size;

	//共享映射区
	void* mmap_addr = mmap(NULL, file_len, PROT_WRITE | PROT_READ, MAP_SHARED, file_fd, 0);
	if (mmap_addr == MAP_FAILED)
	{
		perror("mmap error");
		return -1;
	}
	close(file_fd);

	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork error");
	}
	else if (pid > 0)
	{
		memcpy(mmap_addr, "hello world", strlen("hello world"));
		wait(NULL);
	}
	else if (pid == 0)
	{
		sleep(1);
		printf("%s", (char*)mmap_addr);
	}

	return 0;
}