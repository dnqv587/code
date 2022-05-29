#pragma once
#include "../thread/mutex.h"
#include <memory>
#include <string>

class AppendFile;
class LogFile :public noncopyable
{
public:
	LogFile(const std::string& baseName, off_t rollSize, bool threadSafe, int flushInterval = 3, int checkEveryN = 1024);

	~LogFile() = default;
	//���������־
	void append(const char* logLine, int len);
	void append(const std::string& logLine)
	{
		this->append(logLine.c_str(), logLine.length());
	}
	//ˢ�»�����
	void flush();
	//ˢ����־�ļ�
	//��Ҫˢ���ļ��㣺������һ�졢�ﵽ���д����
	bool rollFile();

private:
	//���������־
	void append_unlocked(const char* logLine, int len);
	//��ʽ����־�ļ���---���ʱ�䡢����ID���߳�ID�ͺ�׺;�������ļ�ʱ��--now
	static std::string getLogFileName(const std::string baseName, time_t* now);

	const std::string m_baseName;//��־����
	const off_t m_rollSize;//����־�ļ����д����־����
	const int m_flashInterval;//ˢ�¼��--��λ��
	const int m_checkEveryN;//���������洢��־����--ÿ��ˢ���ļ��򻺳�������־����

	time_t m_startPeriod;//��¼һ��Ŀ�ʼʱ��--��λ��
	time_t m_lastRoll;//���һ��ˢ���ļ���ʱ��--��λ��
	time_t m_lastFlush;//���һ��ˢ�»�������ʱ��--��λ��

	size_t m_logCount;//��д�����־����--���checkEveryN
	std::unique_ptr<MutexLock> m_mutex;
	std::unique_ptr<AppendFile> m_file;	
};
