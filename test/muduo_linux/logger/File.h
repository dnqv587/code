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

	//��д��size
	off_t writtenBytes()
	{
		return m_writtenBytes;
	}

private:
	size_t write(const char* logLine, size_t len);//����д��---���̰߳�ȫ

	FILE* m_file;
	char m_buf[MAX_BUFFER_SIZE];//�û�̬������ �����ļ�IO�Ĵ���
	off_t m_writtenBytes;
};