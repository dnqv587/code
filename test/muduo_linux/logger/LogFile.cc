#include "LogFile.h"
#include "../base/ProcessInfo.h"
#include <assert.h>
#include <time.h>

constexpr int kRollPerSeconds = 60 * 60 * 24;//一天的秒数

LogFile::LogFile(const std::string& baseName, off_t rollSize, bool threadSafe, int flushInterval /*= 3*/, int checkEveryN /*= 1024*/)
	:m_baseName(baseName), m_rollSize(rollSize), m_mutex(threadSafe ? new MutexLock : nullptr), m_flashInterval(flushInterval), m_checkEveryN(checkEveryN), m_logCount(0), m_startPeriod(0), m_lastRoll(0), m_lastFlush(0)
{
	assert(m_baseName.find('/') == std::string::npos);//检测文件名
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
	time_t start = now / kRollPerSeconds * kRollPerSeconds; //相当于 now-(now%kRollPerSeconds)

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
	if (m_file->writtenBytes() > m_rollSize)//已写入的字节大于最大写入数
	{
		this->rollFile();//换日志文件
	}
	else
	{
		++m_logCount;
		if (m_logCount >= m_checkEveryN)//刷新文件或缓冲区检查点
		{
			m_logCount = 0;
			time_t nowSec = ::time(NULL);//当前时间
			time_t curPeriod = nowSec / kRollPerSeconds * kRollPerSeconds;
			if (curPeriod != this->m_startPeriod)//进入下一天
			{
				this->rollFile();
			}
			else if (nowSec - m_lastFlush > m_flashInterval)//需要刷新缓冲区
			{
				m_lastFlush = nowSec;
				m_file->flush();//无锁

			}
		}
	}
}

std::string LogFile::getLogFileName(const std::string baseName, time_t* now)
{
	//添加日志名
	std::string fileName;
	fileName.reserve(baseName.size() + 64);//预分配内存空间
	fileName.assign(baseName);
	//添加时间
	char timeBuf[32];
	struct tm tm;
	*now = time(NULL);
	::gmtime_r(now, &tm);
	::strftime(timeBuf, sizeof(timeBuf), ".%Y%m%d-%H%M%S.", &tm);
	fileName.append(timeBuf);
	//添加进程信息
	fileName.append(ProcessInfo::hostname());
	fileName.push_back('.');
	fileName.append(ProcessInfo::pidString());
	//后缀
	fileName.append(".log");
	
	return fileName;
}
