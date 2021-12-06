#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct node
{
	int data;
	struct node* next;
}NODE;

NODE* head = NULL;
void* producer(void* args)
{
	NODE* pNODE = NULL;
	while (1)
	{
		pNODE = (NODE*)malloc(sizeof(NODE));
		if (pNODE == NULL)
		{
			perror("malloc error");
			exit(-1);
		}
		pNODE->data = rand() % 1000;
		printf("P:[%d]\n", pNODE->data);
		head = pNODE;
		head->next = pNODE;

		/*pNODE->next = head;
		head = pNODE;*/
		sleep(1);

	}
}

void* consumer(void* args)
{
	NODE* pNODE = NULL;
	while (1)
	{
		printf("C:[%d]\n", head->data);
		pNODE = head;
		head = head->next;
		free(pNODE);
		pNODE = NULL;
	}
}

int main(int argc, char* argv[])
{
	//�߳�ID
	pthread_t producer_thread;
	pthread_t consumer_thread;

	//�����߳�����
	//pthread_attr_t producer_attr;
	//pthread_attr_t consumer_attr;

	//pthread_attr_init(&producer_attr);
	//pthread_attr_init(&consumer_attr);

	//���÷�������
	//pthread_attr_setdetachstate(&producer_attr, PTHREAD_CREATE_DETACHED);
	//pthread_attr_setdetachstate(&consumer_attr, PTHREAD_CREATE_DETACHED);

	//�����߳�
	if (pthread_create(&producer_thread, NULL, &producer, NULL) != 0)
	{
		perror("pthread_create error");
	}
	if (pthread_create(&consumer_thread, NULL, &consumer, NULL) != 0)
	{
		perror("pthread_create error");
	}

	//�ȴ��߳̽���
	pthread_join(producer_thread, NULL);
	pthread_join(consumer_thread, NULL);

	//�����߳�����
	//pthread_attr_destroy(&producer_attr);
	//pthread_attr_destroy(&consumer_attr);
}