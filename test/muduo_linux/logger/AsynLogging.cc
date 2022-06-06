#include "AsynLogging.h"
#include "assert.h"
#include "LogFile.h"
#include "../time/Timestamp.h"

AsynLogging::AsynLogging(std::string& baseName, off_t rollSize, int flushInterval /*= 3*/)
	:m_baseName(baseName),m_rollSize(rollSize),m_flushInterval(flushInterval), m_running(false),
	m_thread(std::bind(threadFunc,this),"logging"), m_mutex(),m_cond(m_mutex), m_latch(1),
	currentBuffer(new Buffer), nextBuffer(new Buffer),m_buffers()
{
	currentBuffer->bzero();
	nextBuffer->bzero();
	m_buffers.reserve(16);//Ԥ���ռ�
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
	if (currentBuffer->avail() > len)//ʣ�໺������������
	{
		currentBuffer->append(logLine, len);
	}
	else
	{
		m_buffers.push_back(std::move(currentBuffer));//���������Ļ�����push��m_buffers��
		if (nextBuffer)
		{
			currentBuffer = std::move(nextBuffer);//��Ԥ���������ƶ�Ϊ��ǰ������
		}
		else
		{
			currentBuffer.reset(new Buffer);//newһ���µĻ�����Ϊ��ǰ������
		}
		currentBuffer->append(logLine, len);
		m_cond.notify();//֪ͨ�첽��־�߳�
	}
}

void AsynLogging::threadFunc()
{
	assert(m_running == true);
	m_latch.countDown();
	LogFile output(m_baseName, m_rollSize, false);
	//Ԥ��������
	BufferPtr newBuffer1(new Buffer);
	BufferPtr newBuffer2(new Buffer);
	newBuffer1->bzero();
	newBuffer2->bzero();
	BufferVector buffersToWrite;//��д�뻺��������
	buffersToWrite.reserve(16);
	
	while (m_running)
	{
		assert(newBuffer1 && newBuffer1->lenght() == 0);
		assert(newBuffer2 && newBuffer2->lenght() == 0);
		assert(buffersToWrite.empty());
		//�ٽ���
		{
			MutexLockGuard lock(m_mutex);
			if (m_buffers.empty())//�Ƿ���������д�뻺��
			{
				m_cond.waitTime(m_flushInterval);//�����Ƿ���������д�뻺�壬���ڵȴ�m_flushIntervalʱ��󶼽�������ˢ���ļ�
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

		if (buffersToWrite.size() > 25)//��־�ѻ����������������ٶȴ��������������ٶȣ���������־
		{
			char buf[256];
			snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
				Timestamp::now().formatString().c_str(),
				buffersToWrite.size() - 2);
			//fputs(buf, stderr);
			::fwrite(buf, 1, sizeof(buf), stderr);
			output.append(buf, static_cast<int>(strlen(buf)));
		}
		//д����־�ļ�
		for (const auto& buffer : buffersToWrite)
		{
			output.append(buffer->data(), buffer->lenght());
		}

		if (buffersToWrite.size() > 2)
		{
			// drop non-bzero-ed buffers, avoid trashing
			buffersToWrite.resize(2);
		}
		//����д��Ļ������黹
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

