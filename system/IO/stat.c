#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{
	struct stat st;
	stat(argv[1],&st);
	printf("dev:%d,uid:%d,size:%d,mode:%X\n",st.st_dev,st.st_uid,st.st_size,st.st_mode);
	return 0;
}
