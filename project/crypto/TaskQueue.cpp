#include "TaskQueue.h"

TaskQueue::TaskQueue()
{
	pthread_mutex_init(&m_mutex, NULL);
}

TaskQueue::~TaskQueue()
{
	pthread_mutex_destroy(&m_mutex);
}

void TaskQueue::addTask(Task& task)
{
	pthread_mutex_lock(&m_mutex);
	m_queue.push(task);
	pthread_mutex_unlock(&m_mutex);
}

void TaskQueue::addTask(callback func, void* arg)
{
	pthread_mutex_lock(&m_mutex);
	Task task;
	task.arg = arg;
	task.function = func;
	m_queue.push(task);
	pthread_mutex_unlock(&m_mutex);
}

Task TaskQueue::tackTask()
{
	if (!isEmpty())
	{
		pthread_mutex_lock(&m_mutex);
		Task task = m_queue.front();
		m_queue.pop();
		pthread_mutex_unlock(&m_mutex);
		return task;
	}
	std::cerr << "TaskQueue is empty!" << std::endl;
	return Task();
}

inline bool TaskQueue::isEmpty()
{
	return !m_queue.size();
}


