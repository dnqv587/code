#include "File.h"
#include <assert.h>
#include <string.h>

AppendFile::AppendFile(const char* fileName) :m_file(::fopen(fileName, "ae")), m_writtenBytes(0)
{//e:�����ļ�O_CLOEXEC���ԣ���ֹ��ֹ���ӽ��̼̳�
	assert(m_file);
	::setbuffer(m_file, m_buf, sizeof(m_buf));//�Զ���stream�Ļ�����
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
		
		if (n != remain)//û��ʣ���ַ���д��
		{
			int err = ferror(m_file);
			if (err)//�����˴���
			{
				fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
				break;
			}
		}
		written += n;
		remain = len - written;
	}
	m_writtenBytes += written;
}

void AppendFile::flush()
{
	::fflush(m_file);//ˢ�»�����
}

inline size_t AppendFile::write(const char* logLine, size_t len)
{
	return ::fwrite_unlocked(logLine, 1, len, m_file);
}

