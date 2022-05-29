#include "LogFile.h"
#include "File.h"
#include "../base/ProcessInfo.h"
#include <assert.h>
#include <time.h>

constexpr int kRollPerSeconds = 60 * 60 * 24;//һ�������

LogFile::LogFile(const std::string& baseName, off_t rollSize, bool threadSafe, int flushInterval /*= 3*/, int checkEveryN /*= 1024*/)
	:m_baseName(baseName), m_rollSize(rollSize), m_mutex(threadSafe ? new MutexLock : nullptr), m_flashInterval(flushInterval), m_checkEveryN(checkEveryN), m_logCount(0), m_startPeriod(0), m_lastRoll(0), m_lastFlush(0)
{
	assert(m_baseName.find('/') == std::string::npos);//����ļ���
	this->rollFile();
}

void LogFile::append(const char* logLine, int len)
{
	if (m_mutex)
	{
		MutexLockGuard lock(*m_mutex);
		this->append_unlocked(logLine, len);
	}
	else
	{
		this->append_unlocked(logLine, len);
	}
}

void LogFile::flush()
{
	if (m_mutex)
	{
		MutexLockGuard lock(*m_mutex);
		m_file->flush();
	}
	else
	{
		m_file->flush();
	}
}

bool LogFile::rollFile()
{
	time_t now = 0;
	std::string fileName = this->getLogFileName(m_baseName, &now);
	time_t start = now / kRollPerSeconds * kRollPerSeconds; //�൱�� now-(now%kRollPerSeconds)

	if (now > m_lastRoll)
	{
		m_lastRoll = now;
		m_lastFlush = now;
		m_startPeriod = start;
		m_file.reset(new AppendFile(fileName.c_str()));
		return true;
	}
	return false;
}

void LogFile::append_unlocked(const char* logLine, int len)
{
	m_file->append(logLine, len);
	if (m_file->writtenBytes() > m_rollSize)//��д����ֽڴ������д����
	{
		this->rollFile();//����־�ļ�
	}
	else
	{
		++m_logCount;
		if (m_logCount >= m_checkEveryN)//ˢ���ļ��򻺳�������
		{
			m_logCount = 0;
			time_t nowSec = ::time(NULL);//��ǰʱ��
			time_t curPeriod = nowSec / kRollPerSeconds * kRollPerSeconds;
			if (curPeriod != this->m_startPeriod)//������һ��
			{
				this->rollFile();
			}
			else if (nowSec - m_lastFlush > m_flashInterval)//��Ҫˢ�»�����
			{
				m_lastFlush = nowSec;
				m_file->flush();//����

			}
		}
	}
}

std::string LogFile::getLogFileName(const std::string baseName, time_t* now)
{
	//�����־��
	std::string fileName;
	fileName.reserve(baseName.size() + 64);//Ԥ�����ڴ�ռ�
	fileName.assign(baseName);
	//���ʱ��
	char timeBuf[32];
	struct tm tm;
	*now = time(NULL);
	::gmtime_r(now, &tm);
	::strftime(timeBuf, sizeof(timeBuf), ".%Y%m%d-%H%M%S.", &tm);
	fileName.append(timeBuf);
	//��ӽ�����Ϣ
	fileName.append(ProcessInfo::hostname());
	fileName.push_back('.');
	fileName.append(ProcessInfo::pidString());
	//��׺
	fileName.append(".log");
	
	return fileName;
}
