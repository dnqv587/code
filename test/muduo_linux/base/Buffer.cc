#include "Buffer.h"
#include <assert.h>
#include "../net/Socket.h"


constexpr size_t kCheapPrepend = 8;//buf预留包头长度

Buffer::Buffer(size_t initialSize /*= kInitialSize*/)
	:m_buffer(kCheapPrepend+ initialSize),
	m_readerIndex(kCheapPrepend),
	m_writeIndex(kCheapPrepend)
{
	assert(readableBytes() == 0);
	assert(writableBytes() == kInitialSize);
	assert(prependableBytes() == kCheapPrepend);
}

ssize_t Buffer::readFd(int sockfd, int* saveError)
{
	//采用双缓冲，栈内存为第二缓冲区，若第一缓冲区不足才使用第二缓冲区
	char extrabuf[65536];
	struct iovec vec[2];
	const size_t writable = writableBytes();
	vec[0].iov_base = beginWrite();
	vec[0].iov_len = writable;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof extrabuf;
	const ssize_t n = Socket::readv(sockfd, vec, 2);
	if (n < 0)
	{
		*saveError = errno;
	}
	else if (implicit_cast<size_t>(n) <= writable)
	{
		m_writeIndex += n;
	}
	else
	{
		m_writeIndex = m_buffer.size();
		append(extrabuf, n - writable);
	}
	return n;
}


void Buffer::append(const char* data, size_t len)
{
	ensureWritableBytes(len);
	std::copy(data, data + len, beginWrite());
	hasWritten(len);
}

void Buffer::ensureWritableBytes(size_t len)
{
	if (writableBytes() < len)
	{
		makeSpace(len);
	}
	assert(writableBytes() >= len);
}

void Buffer::makeSpace(size_t len)
{
	if (writableBytes() + prependableBytes() < len + kCheapPrepend)//可写容量不足
	{
		m_buffer.resize(m_writeIndex + len);//增加此size
	}
	else //进行滑动窗口
	{
		assert(kCheapPrepend < m_readerIndex);
		size_t readable = readableBytes();
		std::copy(begin() + m_readerIndex, begin() + m_writeIndex, begin() + kCheapPrepend);
		m_readerIndex = kCheapPrepend;
		m_writeIndex = m_readerIndex + readable;
		assert(readable == readableBytes());
	}
}

void Buffer::hasWritten(size_t len)
{
	assert(len <= writableBytes());
	m_writeIndex += len;
}
