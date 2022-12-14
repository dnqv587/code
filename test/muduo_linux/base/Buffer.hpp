#pragma once
#include "copyable.h"
#include <vector>

constexpr size_t kInitialSize = 1024;//默认buf大小
constexpr size_t kCheapPrepend = 8;//buf预留长度
class Buffer :public copyable
{
public:
	explicit Buffer(size_t initialSize = kInitialSize)
		:m_buffer(initialSize + kCheapPrepend),
		m_readerIndex(kCheapPrepend),
		m_writeIndex(kCheapPrepend)
	{

	}

private:
	std::vector<char> m_buffer;
	size_t m_readerIndex;
	size_t m_writeIndex;
};