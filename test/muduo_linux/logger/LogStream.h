#pragma once

#include "../base/noncopyable.h"
#include <string>
#include <string.h>
#include "../base/Type.h"

//log数据流

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
	int lenght() const { return static_cast<int> (m_cur - m_data); }
	//返回可用大小
	int avail() const { return static_cast<int> (this->end() - m_cur); }
	//重置指针
	void reset() { m_cur = m_data; }
	//清空数据
	void bzero() 
	{ 
		//memset(m_data, 0x00, sizeof(m_data)); 
		memZero(m_data, sizeof(m_data));
	}
	
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
	/*
	Buffer& operator=(const Buffer& buf)
	{
		this->m_data = buf.m_data;
		this->m_cur = buf.m_cur;
		return *this;
	}*/
	Buffer& operator+=(const int n)
	{
		m_cur += n;
		return *this;
	}

private:
	//返回数据尾指针
	const char* end() const { return m_data + SIZE; }

private:
	char m_data[SIZE];//数据

	char* m_cur;//指针
};


class format;
class LogStream:public noncopyable
{
public:
	typedef Buffer<SMALL_BUFFERSIZE> BUFFER;

	LogStream()
	{
	}
	~LogStream() {}

	LogStream& operator<<(bool );

	LogStream& operator<<(short);
	LogStream& operator<<(unsigned short);
	LogStream& operator<<(int);
	LogStream& operator<<(unsigned int);
	LogStream& operator<<(long);
	LogStream& operator<<(unsigned long);
	LogStream& operator<<(long long);
	LogStream& operator<<(unsigned long long);

	LogStream& operator<<(const void*);

	LogStream& operator<<(float);
	LogStream& operator<<(double);

	LogStream& operator<<(char);
	LogStream& operator<<(const char* );
	LogStream& operator<<(const unsigned char* );
	LogStream& operator<<(const std::string& );
	LogStream& operator<<(const BUFFER& );
	LogStream& operator<<(const format&);

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
	const BUFFER& buffer()
	{
		return m_buffer;
	}

private:
	//integer转string
	template<typename T>
	void formatInteger(T v);
	//静态检查
	void staticCheck();
	
	BUFFER m_buffer;
};


class format :public noncopyable
{
public:
	template <typename T>
	format(const char* fmt, T val)
	{
		//判断模板是否为算数类型
		static_assert(std::is_arithmetic<T>::value == true, "Must be arithmetic type");

		m_len = snprintf(m_buf, sizeof(m_buf), fmt, val);
		//assert(static_cast<size_t>(m_len) < sizeof(m_buf));
	}

	const char* data() const
	{
		return m_buf;
	}

	int len() const
	{
		return m_len;
	}

private:
	char m_buf[48];
	int m_len;

};

