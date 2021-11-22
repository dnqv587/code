//基于epoll模型的web服务器

#include "wrap.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <signal.h>

#define MAX_COUNT 1024

struct Line
{
	char reqType[16];
	char fileName[256];
	char protocal[16];
};

struct request
{
	struct Line line;
};

//"编码"，用作回写浏览器的时候，将除字母数字及/_.-~以外的字符转义后回写。
//strencode(encoded_name, sizeof(encoded_name), name);
void strencode(char* to, size_t tosize, const char* from)
{
	int tolen;

	for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from) {
		if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0) {
			*to = *from;
			++to;
			++tolen;
		}
		else {
			sprintf(to, "%%%02x", (int)*from & 0xff);
			to += 3;
			tolen += 3;
		}
	}
	*to = '\0';
}

//16进制数转化为10进制, return 0不会出现
int hexit(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}



//文件名解码
void strdecode(char* to, char* from)
{
	for (; *from != '\0'; ++to, ++from) {

		if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2])) { //依次判断from中 %20 三个字符

			*to = hexit(from[1]) * 16 + hexit(from[2]);//字符串E8变成了真正的16进制的E8
			from += 2;                      //移过已经处理的两个字符(%21指针指向1),表达式3的++from还会再向后移一个字符
		}
		else
			*to = *from;
	}
	*to = '\0';
}

//判断文件类型
char* get_mime_type(char* name)
{
	char* dot;

	dot = strrchr(name, '.');	//自右向左查找‘.’字符;如不存在返回NULL

	if (dot == (char*)0)
		return "text/plain; charset=utf-8";
	if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
		return "text/html; charset=utf-8";
	if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
		return "image/jpeg";
	if (strcmp(dot, ".gif") == 0)
		return "image/gif";
	if (strcmp(dot, ".png") == 0)
		return "image/png";
	if (strcmp(dot, ".css") == 0)
		return "text/css";
	if (strcmp(dot, ".au") == 0)
		return "audio/basic";
	if (strcmp(dot, ".wav") == 0)
		return "audio/wav";
	if (strcmp(dot, ".avi") == 0)
		return "video/x-msvideo";
	if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
		return "video/quicktime";
	if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
		return "video/mpeg";
	if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
		return "model/vrml";
	if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
		return "audio/midi";
	if (strcmp(dot, ".mp3") == 0)
		return "audio/mpeg";
	if (strcmp(dot, ".ogg") == 0)
		return "application/ogg";
	if (strcmp(dot, ".pac") == 0)
		return "application/x-ns-proxy-autoconfig";

	return "text/plain; charset=utf-8";
}

//发送状态行、消息报头和空行
int send_header(int fd,char *code,char *msg,char *fileType,int length)
{
	char buf[1024] = { 0 };
	sprintf(buf, "HTTP/1.1 %s %s\r\n", code, msg);
	sprintf(buf + strlen(buf), "Content-Type:%s\r\n", fileType);
	if (length > 0)
	{
		sprintf(buf + strlen(buf), "Content-Length:%d\r\n", length);
	}
	strcat(buf, "\r\n");
	Write(fd, buf, strlen(buf));
	return 0;
}

//发送响应正文
int send_file(int fd, char* fileName)
{
	int ffd = open(fileName, O_RDONLY);
	if (ffd < 0)
	{
		perror("open error");
		return -1;
	}
	//循环读文件然后发送
	char buf[1024];
	while (1)
	{
		memset(buf, 0x00, sizeof(buf));
		int n = Read(ffd, buf, sizeof(buf));
		if (n <= 0)
		{
			break;
		}
		Write(fd, buf, n);
	}
}


//读取一行
ssize_t readLine(int fd,void* ptr,size_t size)
{
	char* vptr = ptr;
	char buf[1024];
	int n = Read(fd, buf, sizeof(buf));
	if (n < 0)
		return -1;
	if (n == 0)
		return 0;
	for (int i = 0; i < n; i++)
	{
		*vptr++ = buf[i];
		if (buf[i] == '\n')
			return i;
	}
	
}

int http_request(int fd,int epfd)
{
	char path[1024] = { 0 };
	struct request http;
	char buf[1024];
	memset(buf, 0x00, sizeof(buf));
	//读取一行
	if (readLine(fd, buf, sizeof(buf)) <= 0)
	{
		//关闭链接
		Close(fd);
		//下树
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
		return -1;
	}
	//格式化请求行
	sscanf(buf, "%s %s %s", http.line.reqType, http.line.fileName, http.line.protocal);
	printf("%s\n", http.line.reqType);
	printf("%s\n", http.line.fileName);
	printf("%s\n", http.line.protocal);
	//转换汉字编码
	strdecode(http.line.fileName, http.line.fileName);
	sprintf(path, "/mnt/hgfs/network/webpath%s", http.line.fileName);
	//printf("%s\n", path);
	//请求资源
	struct stat st;
	//文件不存在
	if (stat(path, &st) < 0)
	{
		printf("file not exist\n");
		//发送头部信息
		send_header(fd, "404", "Not Found", get_mime_type("error.html"), 0);
		//发送文件内容
		send_file(fd, "/mnt/hgfs/network/webpath/error.html");
	}
	else//文件存在
	{
		//判断文件类型
		if (S_ISREG(st.st_mode))//普通文件
		{
			//发送头部信息
			send_header(fd, "200", "OK", get_mime_type(http.line.fileName), st.st_size);
			//发送文件内容
			send_file(fd, path);
		}
		else if (S_ISDIR(st.st_mode))//目录文件
		{
			printf("目录文件\n");
			//发送头部信息
			send_header(fd, "200", "OK", get_mime_type(".html"), 0);
			//发送html文件头部
			send_file(fd, "/mnt/hgfs/network/webpath/html/dir_header.html");
			//文件列表信息
			char buffer[1024];
			struct dirent** namelist;
			int num = scandir(path, &namelist, NULL, alphasort);
			if (num < 0)
			{
				perror("scandir");
				close(fd);
				epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
				return -1;

			}
			else
			{
				while (num--)
				{
					printf("%s\n", namelist[num]->d_name);
					memset(buffer, 0x00, sizeof(buffer));
					if (namelist[num]->d_type == DT_DIR)//是否为目录
					{
						sprintf(buffer, "<li><a href=%s/>%s</a></li>", namelist[num]->d_name, namelist[num]->d_name);
					}
					else
					{
						sprintf(buffer, "<li><a href=%s>%s</a></li>", namelist[num]->d_name, namelist[num]->d_name);
					}
					free(namelist[num]);
					Write(fd, buffer, strlen(buffer));
				}
				free(namelist);
			}
			//发送html文件尾部
			//send_file(fd, "/mnt/hgfs/network/webpath/html/dir_tail.html");
		}

	}
}

int main(int argc, char** argv)
{
	//若web服务器给浏览器发送数据的时候, 浏览器已经关闭连接, 
	//则web服务器就会收到SIGPIPE信号
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);
	//创建socket
	int lfd = Socket(PF_INET, SOCK_STREAM, 0);
	//设置端口复用
	const int reuse = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	//绑定端口
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
	//监听
	Listen(lfd, 1024);
	//创建epoll树根
	int epfd = epoll_create(1024);
	if (epfd < 0)
	{
		perror("epoll_create error");
		Close(lfd);
		return -1;
	}
	//将lfd上epoll树
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = lfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	//返回事件
	struct epoll_event events[MAX_COUNT];
	//客户端信息
	struct sockaddr_in client;
	//循环
	while (1)
	{
		//等待事件发生
		int nready = epoll_wait(epfd, events, MAX_COUNT, -1);
		if (nready < 0)
		{
			if (errno == EINTR)
				continue;
			printf("epoll wait error ");
			break;
		}
		for (int i = 0; i < nready; i++)
		{
			int fd = events[i].data.fd;
			//有客户端请求
			if (fd == lfd)
			{
				int cfd = Accept(lfd, NULL, NULL);
				//设置cfd为非阻塞模式
				int flag = fcntl(cfd, F_GETFL);
				flag |= O_NONBLOCK;
				fcntl(cfd, F_SETFL, flag);
				//将cfd加入epll根
				ev.events = EPOLLIN;
				ev.data.fd = cfd;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
				continue;
			}
			//有客户端数据发来
			http_request(fd,epfd);

		}
	}
}