#pragma once

#include "../base/noncopyable.h"
#include <sys/types.h>
#include <stdio.h>

constexpr int MAX_BUFFER_SIZE = 64 * 1024;//64K


class AppendFile:public noncopyable
{
public:
	AppendFile(const char* fileName);

	~AppendFile();

	void append(const char* logLine, size_t len);

	void flush();

	//已写入size
	off_t writtenBytes()
	{
		return m_writtenBytes;
	}

private:
	size_t write(const char* logLine, size_t len);//无锁写入---非线程安全

	FILE* m_file;
	char m_buf[MAX_BUFFER_SIZE];//用户态缓冲区 减少文件IO的次数
	off_t m_writtenBytes;
};