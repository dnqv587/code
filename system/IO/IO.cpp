#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <string.h>

using namespace std;


void runcmd(const char * cwd_path);
void create_text(const char * cwd_path)
{
	printf("请输入文件名：");
	string file_name;
	cin >> file_name;
	printf("%s\n", file_name.c_str());
	char file_path[1024];
	sprintf(file_path, "%s/%s.txt", cwd_path,file_name.c_str());
	printf("%s\n", file_path);
	int file_fd = open(file_path, O_RDWR | O_CREAT | O_APPEND, 0775);
	if (file_fd < 0)
	{
		perror("create error");
	}
	char str[1024];
	while (1)
	{
		printf("往文件写内容，输入99退出：");
		cin >> str;
		if (str[0] == '9'&& str[1]=='9')
			break;
		if ((write(file_fd, str, strlen(str)) < 0))
		{
			perror("write error");
		}
		if ((write(file_fd, "\r\n", 2) < 0))
		{
			perror("write error");
		}
	}
	close(file_fd);
	runcmd(cwd_path);
}


void runcmd(const char * cwd_path)
{
	printf("新建文件请按 1 \n");
	printf("修改文件请按 2 \n");
	printf("删除文件请按 3 \n");
	printf("please input  1 to 3:");
	int flag;
	while (cin >> flag)
	{
		if (flag >= 1 && flag <= 3)
		{
			break;
		}
		printf("input error \n");
		printf("please input  1 to 3:");
	}
	switch (flag)
	{
	case 1:
		create_text(cwd_path);
		break;
	case 2:
		//change_text();
		break;
	case 3:
		//delete_text();
		break;
	}
}

int main(int argc, char* argv[])
{
	char *cwd_path;
	if ((cwd_path=getcwd(NULL, 0)) == NULL)
	{
		perror("getcwd error");
	}
	printf("%s\n", cwd_path);
	runcmd(cwd_path);

	free(cwd_path);
	return 0;
}