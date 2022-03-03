#pragma once
#include <iostream>
#include <pthread.h>
#include <queue>
#include <string>


/*任务队列*/

struct Task
{
	Task()
	{
		function = nullptr;
		arg = nullptr;
	}

	void* (*function)(void*);//函数指针，回调函数
	void* arg;//函数参数
};

class TaskQueue
{
public:
	typedef void* (*callback)(void*);
	TaskQueue();
	~TaskQueue();

	/*添加任务*/
	void addTask(Task& task);
	void addTask(callback func, void* arg);

	/*取出任务*/
	Task tackTask();

	inline bool isEmpty();

	std::size_t taskNumber(){ return m_queue.size(); }

private:
	std::queue<Task> m_queue;//队列

	pthread_mutex_t m_mutex;//互斥锁
};