#pragma once
#include "copyable.h"
#include <sys/types.h>
#include <vector>
#include <string>


/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size

class Buffer :public copyable
{
	static const size_t kInitialSize = 1024;//默认buf大小
	static const size_t kCheapPrepend = 8;//buf预留包头长度
public:
	explicit Buffer(size_t initialSize = kInitialSize);

	ssize_t readFd(int sockfd, int* saveError);
	/// <summary>
	/// 可读大小-单位字节
	/// </summary>
	/// <returns></returns>
	size_t readableBytes() const 
	{
		return m_writeIndex - m_readerIndex;
	}
	/// <summary>
	/// 可读大小-单位字节
	/// </summary>
	/// <returns></returns>
	size_t writableBytes() const
	{
		return m_buffer.size() - m_writeIndex;
	}
	/// <summary>
	/// 预留空间大小-单位字节
	/// </summary>
	/// <returns></returns>
	size_t prependableBytes() const
	{
		return m_readerIndex;
	}

	/// <summary>
	/// 读取字符串
	/// </summary>
	/// <returns></returns>
	//const char* readString()
	//{
	//	const char* buf = peek();
	//	retrieve(m_writeIndex-kInitialSize);
	//	return buf;
	//}

	/// <summary>
	/// 读取buf，不移动偏移值
	/// </summary>
	/// <returns></returns>
	const char* peek() const 
	{
		return beginRead();
	}

	/// <summary>
	/// 移动偏移值
	/// </summary>
	/// <param name="len"></param>
	void retrieve(size_t len)
	{
		if (readableBytes() > len)
		{
			m_readerIndex + len;
		}
		else
		{
			retrieveAll();
		}

	}

	/// <summary>
	/// 移动所有偏移值
	/// </summary>
	void retrieveAll()
	{
		m_readerIndex = kCheapPrepend;
		m_writeIndex = kCheapPrepend;
	}

	/// <summary>
	/// 追加字符串
	/// </summary>
	/// <param name="data"></param>
	/// <param name="len"></param>
	void append(const char* data, size_t len);

	/// <summary>
	/// 保证可写的长度
	/// </summary>
	/// <param name="len">保证的长度</param>
	void ensureWritableBytes(size_t len);

	/// <summary>
	/// 建立空间，采用直接申请内存和滑动窗口
	/// </summary>
	/// <param name="len">长度</param>
	void makeSpace(size_t len);

	/// <summary>
	/// 修改写入后的下标
	/// </summary>
	/// <param name="len">写入长度</param>
	void hasWritten(size_t len);

	/// <summary>
	/// buf基地址
	/// </summary>
	/// <returns></returns>
	char* begin() 
	{
		return m_buffer.begin().base();
	}
	const char* begin() const
	{
		return m_buffer.begin().base();
	}
	/// <summary>
	/// 可写基地址
	/// </summary>
	/// <returns></returns>
	char* beginWrite()
	{
		return begin() + m_writeIndex;
	}
	const char* beginWrite() const
	{
		return begin() + m_writeIndex;
	}
	/// <summary>
	/// 可读基地址
	/// </summary>
	/// <returns></returns>
	const char* beginRead() const
	{
		return begin() + m_readerIndex;
	}

private:
	std::vector<char> m_buffer;
	size_t m_readerIndex;//可读下标
	size_t m_writeIndex;//可写下标
};


