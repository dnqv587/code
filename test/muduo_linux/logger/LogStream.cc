#include "LogStream.h"
#include <algorithm>
#include <limits>
#include <type_traits>
#include <assert.h>
#include <stdint.h>

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
static_assert(sizeof(digits) == 20, "wrong number of digits");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

//高效的integer转string
template<typename T>
size_t convert(char buf[], T value)//十进制
{
	T i = value;
	char* p = buf;

	do
	{
		int lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	} while (i != 0);

	if (value < 0)
	{
		*p++ = '-';
	}
	*p = '\0';
	std::reverse(buf, p);

	return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)//十六进制
{
	uintptr_t i = value;
	char* p = buf;

	do
	{
		int lsd = static_cast<int>(i % 16);
		i /= 16;
		*p++ = digitsHex[lsd];
	} while (i != 0);

	*p = '\0';
	std::reverse(buf, p);

	return p - buf;
}

template<typename T>
void LogStream::formatInteger(T v)
{
	if (m_buffer.avail() > MAXNUMRICSIZE)
	{
		size_t len = convert(m_buffer.current(), v);
		m_buffer += len;
	}
}


void LogStream::staticCheck()
{
	static_assert(MAXNUMRICSIZE - 10 > std::numeric_limits<double>::digits10,
		"MAXNUMRICSIZE is large enough");
	static_assert(MAXNUMRICSIZE - 10 > std::numeric_limits<long double>::digits10,
		"MAXNUMRICSIZE is large enough");
	static_assert(MAXNUMRICSIZE - 10 > std::numeric_limits<long>::digits10,
		"MAXNUMRICSIZE is large enough");
	static_assert(MAXNUMRICSIZE - 10 > std::numeric_limits<long long>::digits10,
		"MAXNUMRICSIZE is large enough");
}

LogStream& LogStream::operator<<(bool v)
{
	m_buffer.append(v ? "1" : "0", 1);
	return *this;
}

LogStream& LogStream::operator<<(short v)
{
	*this << static_cast<int>(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
	*this << static_cast<unsigned int>(v);
	return *this;
}

LogStream& LogStream::operator<<(int v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(const void* p)
{
	uintptr_t v = reinterpret_cast<uintptr_t>(p);
	if (m_buffer.avail() >= MAXNUMRICSIZE)
	{
		char* buf = m_buffer.current();
		buf[0] = '0';
		buf[1] = 'x';
		size_t len = convertHex(buf + 2, v);
		m_buffer += (len + 2);
	}
	return *this;
}

LogStream& LogStream::operator<<(float v)
{
	if (m_buffer.avail() > MAXNUMRICSIZE)
	{
		int len = snprintf(m_buffer.current(), MAXNUMRICSIZE, "% .12g", v);
		m_buffer += len;
	}
	return *this;
}

LogStream& LogStream::operator<<(double v)
{
	if (m_buffer.avail() > MAXNUMRICSIZE)
	{
		int len = snprintf(m_buffer.current(), MAXNUMRICSIZE, "% .12g", v);
		m_buffer += len;
	}
	return *this;
}

LogStream& LogStream::operator<<(char v)
{
	m_buffer.append(&v, 1);
	return *this;
}

LogStream& LogStream::operator<<(const char* str)
{
	m_buffer.append(str, strlen(str));
	return *this;
}

LogStream& LogStream::operator<<(const unsigned char* str)
{
	return *this << reinterpret_cast<const char*>(str);
}

LogStream& LogStream::operator<<(const std::string& v)
{
	return *this << v.c_str();
}

LogStream& LogStream::operator<<(const BUFFER& v)
{
	m_buffer.append(v.data(), v.lenght());
	return *this;
}


LogStream& LogStream::operator<<(const format& fmt)
{
	m_buffer.append(fmt.data(), fmt.len());
	return *this;
}

//template <typename T>
//format::format(const char* fmt, T val)
//{
//	//判断模板是否为算数类型
//	static_assert(std::is_arithmetic<T>::value == true, "Must be arithmetic type");
//
//	m_len = snprintf(m_buf, sizeof(m_buf), fmt, val);
//	assert(static_cast<size_t>(m_len) < sizeof(m_buf));
//}