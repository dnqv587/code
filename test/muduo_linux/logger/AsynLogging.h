#pragma once 
#include "../thread/mutex.h"
#include "../base/noncopyable.h"
#include "../thread/thread.h"
#include "../thread/condition.h"
#include "../thread/CountDownLatch.h"
#include "LogStream.h"
#include <string>
#include <atomic>
#include <vector>


class AsynLogging :public noncopyable
{
public:
	AsynLogging(std::string& baseName, off_t rollSize, int flushInterval = 3);

	~AsynLogging();

	void append(const char* logLine, int len);

	//开启异步日志
	void start()
	{
		m_running = true;
		m_thread.start();
		m_latch.wait();//等待线程启动完毕
	}
	//停止异步日志
	void stop()
	{
		m_running = false;
		m_cond.notify();
		m_thread.join();//回收线程
	}

private:
	void threadFunc();//异步日志线程

	typedef Buffer<LARGE_BUFFERSIZE> Buffer;//4MB
	typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
	typedef BufferVector::value_type BufferPtr;

	std::atomic<bool> m_running;//线程运行标识
	Thread m_thread;//线程
	MutexLock m_mutex;
	Condition m_cond;
	CountDownLatch m_latch;
	const std::string m_baseName;
	const off_t m_rollSize;//文件刷新大小
	const int m_flushInterval;//刷新间隔
	//临界值
	BufferPtr currentBuffer;//双缓冲--当前缓冲
	BufferPtr nextBuffer;//预备缓冲
	BufferVector m_buffers;//待写入文件的已填满的缓冲
};

