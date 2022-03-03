#include "ThreadPool.h"

#ifdef WIN32
#include <windows.h>
#endif

ThreadPool::ThreadPool()
{
	int num_cores;//cpu������Ŀ
#ifdef WIN32
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	num_cores = si.dwNumberOfProcessors;
#else
	num_cores = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*��������߳�ID*/
	for (int i = 0; i < num_cores; ++i)
	{
		pthread_t thread;
		m_threads.emplace_back(thread);
	}
	/*��ʼ�����������ź���*/
	if (pthread_mutex_init(&m_mutex, NULL) != 0 || sem_init(&m_sem, 0, 1) != 0 || pthread_cond_init(&m_cond,NULL) != 0)
	{
		std::cerr << "init semaphore or mutex and cond error" << std::endl;
	}

	/*���������߳�*/
	for (auto thread : m_threads)
	{
		pthread_create(&thread, NULL, threadWorker, this);
		++m_threadNum;
	}

	/*���������߳�*/
	pthread_create(&m_manager, NULL, threadManager, this);

}

ThreadPool::~ThreadPool()
{
	running = false;
	/*ʹ�߳��˳�������*/
	for (int i = 0; i < m_threads.size(); ++i)
	{
		sem_post(&m_sem);//֪ͨ�߳�
	}
	for (auto thread : m_threads)
	{
		pthread_join(thread, NULL);//�����߳�
	}

	/*���ٻ��������ź����Լ���������*/
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
	sem_destroy(&m_sem);
}


void ThreadPool::addPoolTask(Task& task)
{
	m_task.addTask(task);

	pthread_cond_signal(&m_cond);//֪ͨ�����߳�
	sem_post(&m_sem);//֪ͨ�����߳�
}

void* ThreadPool::threadWorker(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	while (true)
	{
		sem_wait(&pool->m_sem);
		if (pool->running == false)//�߳��˳�
		{
			std::cout << "�߳��˳�" << std::endl;
			break;
		}
		
		/*�����������ȡ����*/
		Task task = pool->m_task.tackTask();
		
		++pool->m_busyThreadNum;//æ�߳�++
		std::cout << "�߳�æ��" << pool->m_busyThreadNum << std::endl;
		task.function(task.arg);//ִ������
		--pool->m_busyThreadNum;//æ�߳�--
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
		if (pool->m_threadNum == pool->m_busyThreadNum )//�̶߳�Ϊæ�߳�
		{
			std::cout << "�̣߳�" << pool->m_threadNum << std::endl;
			//�����߳�
			pthread_t thread;
			pool->m_threads.emplace_back(thread);
			pthread_create(&thread, NULL, threadWorker, arg);
			++pool->m_threadNum;
			std::cout << "�����̣߳��߳���Ϊ��" << pool->m_threadNum << std::endl;
		}
		pthread_mutex_unlock(&pool->m_mutex);

	}
}