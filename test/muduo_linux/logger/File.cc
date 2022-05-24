#include "File.h"
#include <assert.h>

AppendFile::AppendFile(const char* fileName) :m_file(::fopen(fileName, "ae")), m_writtenBytes(0)
{
	assert(m_file);
	::setbuffer(m_file, m_buf, sizeof(m_buf));//自定义stream的缓冲区
}

AppendFile::~AppendFile()
{
	::fclose(m_file);
}

void AppendFile::append(const char* logLine, size_t len)
{
	size_t written = 0;
	size_t remain = len - written;
	while (remain)
	{
		size_t n = this->write(logLine, remain);

		if (n != remain)//没将剩余字符串写完
		{
			int err = ferror(m_file);
			if (err)//发生了错误
			{
				fprintf(stderr,"AppendFile::append() failed %s\n",strerror())
			}
		}
	}
}

void AppendFile::flush()
{
	::fflush(m_file);//刷新缓冲区
}

inline size_t AppendFile::write(const char* logLine, size_t len)
{
	return ::fwrite_unlocked(logLine, 1, len, m_file);
}

