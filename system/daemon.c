#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>



int TCP()
{
	//����socket����ü����ļ�������
	int kfd = socket(AF_INET, SOCK_STREAM, 0);
	if (kfd < 0)
	{
		perror("socket error");
		return -1;
	}

	struct sockaddr_in serv;
	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8888);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	//��socket
	int ret = bind(kfd, (struct sockaddr*)&serv, sizeof(serv));
	if (ret < 0)
	{
		perror("bind error");
		return -1;
	}
	//����socket
	listen(kfd, 128);
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	//���������ӣ������ͨ���ļ�������
	int cfd = accept(kfd, (struct sockaddr*)&client, &len);
	int i = 0;
	int n = 0;
	char buf[1024];
	//��д����
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		n = read(cfd, buf, sizeof(buf));
		if (n <= 0)
		{
			printf("read error");
			break;
		}
		printf("%s", buf);
		for (i = 0; i < n; i++)
		{
			buf[i] = toupper(buf[i]);
		}

		write(cfd, buf, n);
	}

	close(kfd);
	close(cfd);


}


int main(int argc, char* argv[])
{
	//�����ӽ��̣��������˳�
	pid_t pid = fork();
	if (pid < 0 || pid>0)
	{
		exit(0);
	}
	//�����Ự
	pid_t sid = setsid();
	//�ı䵱ǰ����Ŀ¼
	chdir("./IO");
	//�ı��ļ�����
	umask(0000);
	//�رձ�׼���롢��׼�������׼����
    close(STDIN_FILENO);
 	close(STDOUT_FILENO);
	close(STDERR_FILENO);

    int lock = open("./daemon.lock",O_RDWR|O_CREAT,0644);
    if(lock < 0)
    {
        perror("open error!");
        exit(1);
    }

    int stat = flock(lock,LOCK_EX|LOCK_NB);
    if(stat < 0)
    {
        perror("another daemon is already running!");
        exit(1);
    }

	TCP();
}
