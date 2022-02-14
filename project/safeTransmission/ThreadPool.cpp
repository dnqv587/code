#include "ThreadPool.h"

#ifdef WIN32
#include <windows.h>
#endif

ThreadPool::ThreadPool()
{
	int num_cores;//cpu核心数目
#ifdef WIN32
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	num_cores = si.dwNumberOfProcessors;
#else
	num_cores = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*容器添加线程ID*/
	for (int i = 0; i < num_cores; ++i)
	{
		pthread_t thread;
		m_threads.emplace_back(thread);
	}
	/*初始化互斥锁和信号量*/
	if (pthread_mutex_init(&m_mutex, NULL) != 0 || sem_init(&m_sem, 0, 1) != 0 || pthread_cond_init(&m_cond,NULL) != 0)
	{
		std::cerr << "init semaphore or mutex and cond error" << std::endl;
	}

	/*创建工作线程*/
	for (auto thread : m_threads)
	{
		pthread_create(&thread, NULL, threadWorker, this);
		++m_threadNum;
	}

	/*创建管理线程*/
	pthread_create(&m_manager, NULL, threadManager, this);

}

ThreadPool::~ThreadPool()
{
	running = false;
	/*使线程退出并回收*/
	for (int i = 0; i < m_threads.size(); ++i)
	{
		sem_post(&m_sem);//通知线程
	}
	for (auto thread : m_threads)
	{
		pthread_join(thread, NULL);//回收线程
	}

	/*销毁互斥锁和信号量以及条件变量*/
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
	sem_destroy(&m_sem);
}


void ThreadPool::addPoolTask(Task& task)
{
	m_task.addTask(task);

	pthread_cond_signal(&m_cond);//通知管理线程
	sem_post(&m_sem);//通知工作线程
}

void* ThreadPool::threadWorker(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	while (true)
	{
		sem_wait(&pool->m_sem);
		if (pool->running == false)//线程退出
		{
			std::cout << "线程退出" << std::endl;
			break;
		}
		
		/*从任务队列拿取任务*/
		Task task = pool->m_task.tackTask();
		
		++pool->m_busyThreadNum;//忙线程++
		std::cout << "线程忙：" << pool->m_busyThreadNum << std::endl;
		task.function(task.arg);//执行任务
		--pool->m_busyThreadNum;//忙线程--
	}

	return nullptr;
}

void* ThreadPool::threadManager(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	while (true)
	{
		pthread_mutex_lock(&pool->m_mutex);
		pthread_cond_wait(&pool->m_cond, &pool->m_mutex);
		if (pool->m_threadNum == pool->m_busyThreadNum )//线程都为忙线程
		{
			std::cout << "线程：" << pool->m_threadNum << std::endl;
			//创建线程
			pthread_t thread;
			pool->m_threads.emplace_back(thread);
			pthread_create(&thread, NULL, threadWorker, arg);
			++pool->m_threadNum;
			std::cout << "创建线程；线程数为：" << pool->m_threadNum << std::endl;
		}
		pthread_mutex_unlock(&pool->m_mutex);

	}
}