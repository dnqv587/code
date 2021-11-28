//IO函数
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types>
#include <sys/stat.h>

int main(int argc,char** argv)
{
	//打开文件
	int fd=open("./open.txt",O_RDWR | O_CREAT,0777);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}
	

}
