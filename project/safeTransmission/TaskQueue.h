#pragma once
#include <iostream>
#include <pthread.h>
#include <queue>
#include <string>


/*�������*/

struct Task
{
	Task()
	{
		function = nullptr;
		arg = nullptr;
	}

	void* (*function)(void*);//����ָ�룬�ص�����
	void* arg;//��������
};

class TaskQueue
{
public:
	typedef void* (*callback)(void*);
	TaskQueue();
	~TaskQueue();

	/*�������*/
	void addTask(Task& task);
	void addTask(callback func, void* arg);

	/*ȡ������*/
	Task tackTask();

	inline bool isEmpty();

	std::size_t taskNumber(){ return m_queue.size(); }

private:
	std::queue<Task> m_queue;//����

	pthread_mutex_t m_mutex;//������
};