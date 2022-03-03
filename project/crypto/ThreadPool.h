#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
#include <atomic>
#include <vector>
#include <semaphore.h>

#include "TaskQueue.h"


/*
	- 管理了多个线程
		- 管理者线程 -> 1个
			- 控制工作线程的个数
				- 发现快不够用了: 创建一批新的线程
				- 不工作的线程太多了: 销毁一批
		- 工作的线程 -> n个
	- 工作线程线程得工作 -> 任务来自于任务队列
		- 工作的线程处理逻辑:
			- 不停的读任务队列
				- 为空: 工作线程阻塞 -> 使用条件变量
				- 不为空: 从任务队列中取出一个任务
					- struct Task -> 任务的类型
					- 调用Task中对应的函数, 进行逻辑处理
		- 当线程阻塞了, 什么时候唤醒?
		   - 往任务队列中添加任务的时候唤醒阻塞线程即可
*/

/*
线程池由三个部分组成
1：执行队列
2：任务队列
3：管理组件
*/


class ThreadPool
{
public:
	ThreadPool();

	~ThreadPool();

	void addPoolTask(Task& task);

private:
	static void* threadWorker(void* arg);//工作线程

	static void* threadManager(void* arg);//管理线程

private:

	std::vector<pthread_t> m_threads;;//工作线程tid容器
	pthread_t m_manager;//管理线程tid
	TaskQueue m_task;//任务队列

	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	sem_t m_sem;
	std::atomic<bool> running = true;//原子操作，线程池是否运行
	std::atomic<int> m_busyThreadNum = 0;//工作线程的个数
	std::atomic<int> m_threadNum = 0;//工作线程个数

};


