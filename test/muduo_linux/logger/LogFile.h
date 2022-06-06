#pragma once
#include "../thread/mutex.h"
#include <memory>
#include <string>

class AppendFile;
class LogFile :public noncopyable
{
public:
	
	/*
	* 日志输出
	* baseName:名称，rollSize：文件刷新字节间隔，threadSafe：是否为线程安全，flushInterval：缓冲区刷新间隔，checkEveryN：缓冲区检查大小
	*/
	LogFile(const std::string& baseName, off_t rollSize, bool threadSafe, int flushInterval = 3, int checkEveryN = 1024);

	~LogFile() = default;
	//加锁输出日志
	void append(const char* logLine, int len);
	void append(const std::string& logLine)
	{
		this->append(logLine.c_str(), logLine.length());
	}
	//刷新缓冲区
	void flush();
	//刷新日志文件
	//需要刷新文件点：进入下一天、达到最大写入数
	bool rollFile();

private:
	//无锁输出日志
	void append_unlocked(const char* logLine, int len);
	//格式化日志文件名---添加时间、进程ID，线程ID和后缀;并传出文件时间--now
	static std::string getLogFileName(const std::string baseName, time_t* now);

	const std::string m_baseName;//日志名称
	const off_t m_rollSize;//单日志文件最大写入日志条数
	const int m_flashInterval;//刷新间隔--单位秒
	const int m_checkEveryN;//缓冲区最大存储日志条数--每次刷新文件或缓冲区的日志条数

	time_t m_startPeriod;//记录一天的开始时间--单位秒
	time_t m_lastRoll;//最后一次刷新文件的时间--单位秒
	time_t m_lastFlush;//最后一次刷新缓冲区的时间--单位秒

	size_t m_logCount;//已写入的日志条数--最大checkEveryN
	std::unique_ptr<MutexLock> m_mutex;
	std::unique_ptr<AppendFile> m_file;	//写日志文件对象，在rollFile进行构造
};
