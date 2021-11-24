#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc,char* argv[])
{
	//打开目录
	DIR* dir=opendir(argv[1]);
	if(dir==NULL)
	{
		perror("opendir error");
		return -1;
	}
	//循环读取目录内容
	struct dirent  *pdir;
	char* type;
	while((pdir=readdir(dir))!=NULL)
	{
		switch(pdir->d_type)
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
		printf("name:%s       type:%s\n",pdir->d_name, type);
	}
	closedir(dir);
	return 0;
}
