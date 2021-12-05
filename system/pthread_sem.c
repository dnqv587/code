#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>

//定义互斥锁
//pthread_mutex_t mutex;
//定义信号量
sem_t sem_producer;
sem_t sem_consumer;

//定义链表结构体
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
		sem_wait(&sem_producer);//消耗一个生产者

		//head = pNODE;
		//head->next = pNODE;

		pNODE->next = head;
		head = pNODE;
		sem_post(&sem_consumer);//增加一个消费者

		sleep(rand()%3);


	}
}

void* consumer(void* args)
{
	NODE* pNODE = NULL;
	while (1)
	{
		sem_wait(&sem_consumer);//消耗一个消费者
		printf("C:[%d]\n", head->data);
		pNODE = head;
		head = head->next;
		free(pNODE);
		pNODE = NULL;
		sem_post(&sem_producer);//增加一个生产者
		sleep(rand()%3);

	}
}

int main(int argc, char* argv[])
{
	//设置随机种子
	srand(time(NULL));
	//线程ID
	pthread_t producer_thread;
	pthread_t consumer_thread;

	//设置线程属性
	//pthread_attr_t producer_attr;
	//pthread_attr_t consumer_attr;

	//pthread_attr_init(&producer_attr);
	//pthread_attr_init(&consumer_attr);

	//设置分离属性
	//pthread_attr_setdetachstate(&producer_attr, PTHREAD_CREATE_DETACHED);
	//pthread_attr_setdetachstate(&consumer_attr, PTHREAD_CREATE_DETACHED);

	//初始化互斥锁
	//pthread_mutex_init(&mutex, NULL);
	//初始化信号量
	sem_init(&sem_producer, 0, 1);
	sem_init(&sem_consumer, 0, 0);

	//创建线程
	if (pthread_create(&producer_thread, NULL, &producer, NULL) != 0)
	{
		perror("pthread_create error");
	}
	if (pthread_create(&consumer_thread, NULL, &consumer, NULL) != 0)
	{
		perror("pthread_create error");
	}

	//等待线程结束
	pthread_join(producer_thread, NULL);
	pthread_join(consumer_thread, NULL);

	//互斥锁回收
	//pthread_mutex_destroy(&mutex);

	//信号量回收
	sem_destroy(&sem_producer);
	sem_destroy(&sem_consumer);

	//回收线程属性
	//pthread_attr_destroy(&producer_attr);
	//pthread_attr_destroy(&consumer_attr);
	return 0;
}