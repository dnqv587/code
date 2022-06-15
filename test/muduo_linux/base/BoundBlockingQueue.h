#pragma once
#include "../thread/mutex.h"
#include "../thread/Condition.h"
#include <queue>

template<typename T>
class BoundBlockingQueue :private noncopyable
{
public:


private:
	MutexLock m_mutex;
	Condition m_cond GUARDED_BY(m_mutex);
	std::queue<T> m_queue GUARDED_BY(m_mutex);
};
