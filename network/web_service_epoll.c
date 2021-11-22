//����epollģ�͵�web������

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

//"����"��������д�������ʱ�򣬽�����ĸ���ּ�/_.-~������ַ�ת����д��
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

//16������ת��Ϊ10����, return 0�������
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



//�ļ�������
void strdecode(char* to, char* from)
{
	for (; *from != '\0'; ++to, ++from) {

		if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2])) { //�����ж�from�� %20 �����ַ�

			*to = hexit(from[1]) * 16 + hexit(from[2]);//�ַ���E8�����������16���Ƶ�E8
			from += 2;                      //�ƹ��Ѿ�����������ַ�(%21ָ��ָ��1),���ʽ3��++from�����������һ���ַ�
		}
		else
			*to = *from;
	}
	*to = '\0';
}

//�ж��ļ�����
char* get_mime_type(char* name)
{
	char* dot;

	dot = strrchr(name, '.');	//����������ҡ�.���ַ�;�粻���ڷ���NULL

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

//����״̬�С���Ϣ��ͷ�Ϳ���
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

//������Ӧ����
int send_file(int fd, char* fileName)
{
	int ffd = open(fileName, O_RDONLY);
	if (ffd < 0)
	{
		perror("open error");
		return -1;
	}
	//ѭ�����ļ�Ȼ����
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


//��ȡһ��
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
	//��ȡһ��
	if (readLine(fd, buf, sizeof(buf)) <= 0)
	{
		//�ر�����
		Close(fd);
		//����
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
		return -1;
	}
	//��ʽ��������
	sscanf(buf, "%s %s %s", http.line.reqType, http.line.fileName, http.line.protocal);
	printf("%s\n", http.line.reqType);
	printf("%s\n", http.line.fileName);
	printf("%s\n", http.line.protocal);
	//ת�����ֱ���
	strdecode(http.line.fileName, http.line.fileName);
	sprintf(path, "/mnt/hgfs/network/webpath%s", http.line.fileName);
	//printf("%s\n", path);
	//������Դ
	struct stat st;
	//�ļ�������
	if (stat(path, &st) < 0)
	{
		printf("file not exist\n");
		//����ͷ����Ϣ
		send_header(fd, "404", "Not Found", get_mime_type("error.html"), 0);
		//�����ļ�����
		send_file(fd, "/mnt/hgfs/network/webpath/error.html");
	}
	else//�ļ�����
	{
		//�ж��ļ�����
		if (S_ISREG(st.st_mode))//��ͨ�ļ�
		{
			//����ͷ����Ϣ
			send_header(fd, "200", "OK", get_mime_type(http.line.fileName), st.st_size);
			//�����ļ�����
			send_file(fd, path);
		}
		else if (S_ISDIR(st.st_mode))//Ŀ¼�ļ�
		{
			printf("Ŀ¼�ļ�\n");
			//����ͷ����Ϣ
			send_header(fd, "200", "OK", get_mime_type(".html"), 0);
			//����html�ļ�ͷ��
			send_file(fd, "/mnt/hgfs/network/webpath/html/dir_header.html");
			//�ļ��б���Ϣ
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
					if (namelist[num]->d_type == DT_DIR)//�Ƿ�ΪĿ¼
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
			//����html�ļ�β��
			//send_file(fd, "/mnt/hgfs/network/webpath/html/dir_tail.html");
		}

	}
}

int main(int argc, char** argv)
{
	//��web��������������������ݵ�ʱ��, ������Ѿ��ر�����, 
	//��web�������ͻ��յ�SIGPIPE�ź�
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);
	//����socket
	int lfd = Socket(PF_INET, SOCK_STREAM, 0);
	//���ö˿ڸ���
	const int reuse = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	//�󶨶˿�
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
	//����
	Listen(lfd, 1024);
	//����epoll����
	int epfd = epoll_create(1024);
	if (epfd < 0)
	{
		perror("epoll_create error");
		Close(lfd);
		return -1;
	}
	//��lfd��epoll��
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = lfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	//�����¼�
	struct epoll_event events[MAX_COUNT];
	//�ͻ�����Ϣ
	struct sockaddr_in client;
	//ѭ��
	while (1)
	{
		//�ȴ��¼�����
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
			//�пͻ�������
			if (fd == lfd)
			{
				int cfd = Accept(lfd, NULL, NULL);
				//����cfdΪ������ģʽ
				int flag = fcntl(cfd, F_GETFL);
				flag |= O_NONBLOCK;
				fcntl(cfd, F_SETFL, flag);
				//��cfd����epll��
				ev.events = EPOLLIN;
				ev.data.fd = cfd;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
				continue;
			}
			//�пͻ������ݷ���
			http_request(fd,epfd);

		}
	}
}