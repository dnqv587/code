#include "AsynLogging.h"
#include "assert.h"
#include "LogFile.h"
#include "../time/Timestamp.h"

AsynLogging::AsynLogging(const char* baseName, off_t rollSize, int flushInterval /*= 3*/)
	:m_baseName(baseName),m_rollSize(rollSize),m_flushInterval(flushInterval), m_running(false),
	m_thread(std::bind(&AsynLogging::threadFunc,this),"logging"), m_mutex(),m_cond(m_mutex), m_latch(1),
	currentBuffer(new BUFFER), nextBuffer(new BUFFER),m_buffers()
{
	currentBuffer->bzero();
	nextBuffer->bzero();
	m_buffers.reserve(16);//预留空间
}

AsynLogging::~AsynLogging()
{
	if (m_running)
	{
		this->stop();
	}
}

void AsynLogging::append(const char* logLine, int len)
{
	MutexLockGuard lock(m_mutex);
	if (currentBuffer->avail() > len)//剩余缓冲区可以容纳
	{
		currentBuffer->append(logLine, len);
	}
	else
	{
		m_buffers.push_back(std::move(currentBuffer));//将已填满的缓冲区push到m_buffers中
		if (nextBuffer)
		{
			currentBuffer = std::move(nextBuffer);//将预备缓冲区移动为当前缓冲区
		}
		else
		{
			currentBuffer.reset(new BUFFER);//new一个新的缓冲区为当前缓冲区
		}
		currentBuffer->append(logLine, len);
		m_cond.notify();//通知异步日志线程
	}
}

void AsynLogging::threadFunc()
{
	assert(m_running == true);
	m_latch.countDown();
	LogFile output(m_baseName, m_rollSize, false);
	//预备缓冲区
	BufferPtr newBuffer1(new BUFFER);
	BufferPtr newBuffer2(new BUFFER);
	newBuffer1->bzero();
	newBuffer2->bzero();
	BufferVector buffersToWrite;//待写入缓冲区容器
	buffersToWrite.reserve(16);
	
	while (m_running)
	{
		assert(newBuffer1 && newBuffer1->lenght() == 0);
		assert(newBuffer2 && newBuffer2->lenght() == 0);
		assert(buffersToWrite.empty());
		//临界区
		{
			MutexLockGuard lock(m_mutex);
			if (m_buffers.empty())//是否有已满待写入缓冲
			{
				m_cond.waitTime(m_flushInterval);//无论是否有已满待写入缓冲，都在等待m_flushInterval时间后都将缓冲区刷入文件
			}
			m_buffers.push_back(std::move(currentBuffer));
			currentBuffer = std::move(newBuffer1);
			std::swap(buffersToWrite, m_buffers);
			if (!nextBuffer)
			{
				newBuffer1 = std::move(newBuffer2);
			}
		}
		assert(!buffersToWrite.empty());

		if (buffersToWrite.size() > 25)//日志堆积，即生产者生产速度大于消费者消费速度，将丢弃日志
		{
			char buf[256];
			snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
				Timestamp::now().formatString().c_str(),
				buffersToWrite.size() - 2);
			//fputs(buf, stderr);
			::fwrite(buf, 1, sizeof(buf), stderr);
			output.append(buf, static_cast<int>(strlen(buf)));
		}
		//写入日志文件
		for (const auto& buffer : buffersToWrite)
		{
			output.append(buffer->data(), buffer->lenght());
		}

		if (buffersToWrite.size() > 2)
		{
			// drop non-bzero-ed buffers, avoid trashing
			buffersToWrite.resize(2);
		}
		//将已写入的缓冲区归还
		if (!newBuffer1)
		{
			assert(!buffersToWrite.empty());
			newBuffer1 = std::move(buffersToWrite.back());
			buffersToWrite.pop_back();
			newBuffer1->reset();
		}
		if (!newBuffer2)
		{
			assert(!buffersToWrite.empty());
			newBuffer2 = std::move(buffersToWrite.back());
			buffersToWrite.pop_back();
			newBuffer2->reset();
		}
		buffersToWrite.clear();
		output.flush();	
	}
	output.flush();
}

