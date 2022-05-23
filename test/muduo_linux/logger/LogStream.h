#pragma once

#include "../base/noncopyable.h"
#include <string>
#include <string.h>

constexpr int SMALL_BUFFERSIZE = 4000;//budder的MIN大小
constexpr int LARGE_BUFFERSIZE = 4000 * 1000;//buffer的MAX大小

constexpr int MAXNUMRICSIZE = 48;//整型转字符串型的最大长度

template<int SIZE>//buffer的大小
class Buffer:public noncopyable
{
public:
	Buffer():m_cur(m_data){}

	~Buffer() {}

	//追加数据
	void append(const char* buf, int len)
	{
		if (this->avail() > len)
		{
			memcpy(m_cur, buf, len);
			m_cur += len;
		}
	}

	const char* data() const { return m_data; }
	char* current() const { return m_cur; }
	//返回数据长度
	int lenght() const { return static_cast<int>m_cur - m_data; }
	//返回可用大小
	int avail() const { return static_cast<int> this->end() - m_cur; }
	//重置指针
	void reset() { m_cur = m_data; }
	//清空数据
	void bzero() { memset(m_data, 0x00, sizeof(m_data)); }
	
	std::string toString() const 
	{
		return std::string(m_data, this->lenght());
	}
	
	Buffer& operator++()
	{
		++m_cur;
		return *this;
	}
	Buffer& operator--()
	{
		--m_cur;
		return *this;
	}
	Buffer& operator+(const int n)
	{
		m_cur += n;
		return *this;
	}
	Buffer& operator-(const int n)
	{
		m_cur -= n;
		return *this;
	}
	Buffer& operator=(const Buffer& buf)
	{
		this->m_data = buf.m_data;
		this->m_cur = buf.m_cur;
		return *this;
	}
	Buffer& operator+=(const int n)
	{
		m_cur += n;
		return *this;
	}

private:
	//返回数据尾指针
	const char* end() { return m_data + SIZE; }

private:
	char m_data[SIZE];//数据

	char* m_cur;//指针
};


class LogStream:public noncopyable
{
	typedef LogStream self;
public:
	typedef Buffer<SMALL_BUFFERSIZE> Buffer;

	self& operator<<(bool v);

	self& operator<<(short);
	self& operator<<(unsigned short);
	self& operator<<(int);
	self& operator<<(unsigned int);
	self& operator<<(long);
	self& operator<<(unsigned long);
	self& operator<<(long long);
	self& operator<<(unsigned long long);

	self& operator<<(const void*);

	self& operator<<(float v);

	self& operator<<(double);

	self& operator<<(char v);
	self& operator<<(const char* str);
	self& operator<<(const unsigned char* str);
	self& operator<<(const std::string& v);
	self& operator<<(const Buffer& v);

	//追加数据
	void append(const char* buf, int len)
	{
		m_buffer.append(buf, len);
	}
	//重置
	void reset()
	{
		m_buffer.reset();
	}
	//返回buffer
	const Buffer& buffer()
	{
		return m_buffer;
	}


private:
	//integer转string
	template<typename T>
	void formatInteger(T v);


	void staticCheck();
	
	Buffer m_buffer;

};

