#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <dirent.h>

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

void change_text(const char * cwd_path)
{
	printf("请输入文件名：");
	string file_name;
	cin >> file_name;
	printf("%s\n", file_name.c_str());
	char file_path[1024];
	sprintf(file_path, "%s/%s.txt", cwd_path, file_name.c_str());
	printf("%s\n", file_path);
	int file_fd = open(file_path, O_RDWR , 0775);
	if (file_fd < 0)
	{
		perror("create error");
	}
	char text_buf[1024];
	int text_num = read(file_fd, text_buf, sizeof(text_buf));
	printf("文件内容：\n%s\n", text_buf);
	lseek(file_fd, 0, SEEK_SET);
	printf("请输入要修改的行数：");
	int row;
	cin >> row;
	printf("\n请输入要修改的列数：");
	int column;
	cin >> column;
	printf("\n请输入要修改的内容：");
	char str[1024];
	cin >> str;
	int r_count = 0;
	int c_count = 0;
	for (int i = 0; i < text_num; ++i)
	{

		if (r_count == row && c_count == column)
		{
			for (int j = 0; j < strlen(str); ++j)
			{
				text_buf[i + j] = str[j];
			}
			break;
		}
		else
		{
			++c_count;
			if (text_buf[i] == '\n')
			{
				++r_count;
				c_count = 0;
			}
		}
	}
	printf("%s", text_buf);
	write(file_fd, text_buf, text_num);
	close(file_fd);
	runcmd(cwd_path);
}

void delete_text(const char * cwd_path)
{
	DIR* dir = opendir(cwd_path);
	if (dir == NULL)
	{
		perror("opendir error");
	}
	//循环读取目录内容
	struct dirent  *pdir;
	char* type;
	while ((pdir = readdir(dir)) != NULL)
	{
		switch (pdir->d_type)
		{
		case DT_BLK:
			type = "块设备";
			break;
		case DT_CHR:
			type = "字符设备";
			break;
		case DT_DIR:
			type = "目录";
			break;
		case DT_LNK:
			type = "软链接";
			break;
		case DT_FIFO:
			type = "管道";
			break;
		case DT_REG:
			type = "普通文件";
			break;
		case DT_SOCK:
			type = "套接字";
			break;
		default:
			type = "未知";
		}
		printf("name:%s       type:%s\n", pdir->d_name, type);
	}
	closedir(dir);
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
		change_text(cwd_path);
		break;
	case 3:
		delete_text(cwd_path);
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