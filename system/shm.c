#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int key = ftok("./IO/text.txt", 1);
	if(key < 0)
	{
		perror("ftok error!");
		return -1;
	}
	int shmid = shmget(key, 4096, IPC_CREAT|0664);
	if(shmid<0)
	{
		perror("shmget error!");
		return -1;
	}
	
	void* memptr = shmat(shmid,NULL,0);
	if(memptr == (void*)-1)
	{
		perror("shmat error!");
		return -1;
	}
	
	memcpy(memptr, "hello world!", sizeof("hello world!"));
	
	pid_t pid=fork();
	if(pid<0)
	{
		perror("fork error");
		return -1;
	}
	else if(pid>0)
	{
		sleep(2);
	}
	else if(pid==0)
	{
		void* memptr = shmat(shmid,NULL,0);
		if(memptr == (void*)-1)
		{
			perror("shmat error!");
			return -1;
		}
		
		printf("get:%s\n", (char*)memptr);
		
		if((shmctl(shmid, IPC_RMID, 0) < 0))
		{
			perror("shmctl error!");
			return -1;
		}	
	}
	
	
	return 0;
}