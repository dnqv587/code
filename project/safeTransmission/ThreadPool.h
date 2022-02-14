#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
#include <atomic>
#include <vector>
#include <semaphore.h>

#include "TaskQueue.h"


/*
	- �����˶���߳�
		- �������߳� -> 1��
			- ���ƹ����̵߳ĸ���
				- ���ֿ첻������: ����һ���µ��߳�
				- ���������߳�̫����: ����һ��
		- �������߳� -> n��
	- �����߳��̵߳ù��� -> �����������������
		- �������̴߳����߼�:
			- ��ͣ�Ķ��������
				- Ϊ��: �����߳����� -> ʹ����������
				- ��Ϊ��: �����������ȡ��һ������
					- struct Task -> ���������
					- ����Task�ж�Ӧ�ĺ���, �����߼�����
		- ���߳�������, ʲôʱ����?
		   - �������������������ʱ���������̼߳���
*/

/*
�̳߳��������������
1��ִ�ж���
2���������
3���������
*/


class ThreadPool
{
public:
	ThreadPool();

	~ThreadPool();

	void addPoolTask(Task& task);

private:
	static void* threadWorker(void* arg);//�����߳�

	static void* threadManager(void* arg);//�����߳�

private:

	std::vector<pthread_t> m_threads;;//�����߳�tid����
	pthread_t m_manager;//�����߳�tid
	TaskQueue m_task;//�������

	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	sem_t m_sem;
	std::atomic<bool> running = true;//ԭ�Ӳ������̳߳��Ƿ�����
	std::atomic<int> m_busyThreadNum = 0;//�����̵߳ĸ���
	std::atomic<int> m_threadNum = 0;//�����̸߳���

};


