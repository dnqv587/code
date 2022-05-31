#include "Timestamp.h"
#include <utility>


constexpr int kMicroSecondsPerSecond = 1000 * 1000;//每秒中微秒单位

inline Timestamp::Timestamp()
	:m_microSecondsSinceEpoch(0)
{

}

inline Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
	:m_microSecondsSinceEpoch(microSecondsSinceEpoch)
{

}

inline bool Timestamp::operator>(const Timestamp& that)
{
	return this->m_microSecondsSinceEpoch > that.m_microSecondsSinceEpoch;
}

bool Timestamp::operator<(const Timestamp& that)
{
	return this->m_microSecondsSinceEpoch < that.m_microSecondsSinceEpoch;
}

inline bool Timestamp::operator==(const Timestamp& that)
{
	return this->m_microSecondsSinceEpoch == that.m_microSecondsSinceEpoch;
}

Timestamp Timestamp::now()
{
	struct timeval tv;
	if (::gettimeofday(&tv, NULL) == 0)//获取当前精确时间
	{
		return Timestamp(tv.tv_sec * kMicroSecondsPerSecond + tv.tv_usec);
	}
	return Timestamp();
}

inline Timestamp Timestamp::fromUnixTime(time_t time, int microSecond)
{
	return Timestamp(time * kMicroSecondsPerSecond + microSecond);
}

inline Timestamp Timestamp::fromUnixTime(time_t time)
{
	return fromUnixTime(time, 0);
}

inline void Timestamp::swap(Timestamp& that)
{
	std::swap(this->m_microSecondsSinceEpoch, that.m_microSecondsSinceEpoch);
}

std::string Timestamp::toString() const
{
	char buf[32] = { 0 };
	int64_t seconds = m_microSecondsSinceEpoch / kMicroSecondsPerSecond;
	int microseconds = m_microSecondsSinceEpoch % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf), "%lld.%06lld", seconds, microseconds);
	return buf;
}

std::string Timestamp::formatString(bool showMicroseconds /*= true*/, bool isLocal /*= false*/) const
{
	char buf[64] = { 0 };
	int64_t seconds = m_microSecondsSinceEpoch / kMicroSecondsPerSecond;
	struct tm timep;
	if (isLocal)
	{
		::localtime_r(&seconds, &timep);
	}
	else
	{
		::gmtime_r(&seconds, &timep);
	}
	
	if (showMicroseconds)
	{
		int microseconds = m_microSecondsSinceEpoch % kMicroSecondsPerSecond;
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
			timep.tm_year + 1900, timep.tm_mon + 1, timep.tm_mday, timep.tm_hour,
			timep.tm_min, timep.tm_sec, microseconds
		);
	}
	else
	{
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
			timep.tm_year + 1900, timep.tm_mon + 1, timep.tm_mday, timep.tm_hour,
			timep.tm_min, timep.tm_sec
		);
	}
	return buf;
}

inline int64_t Timestamp::microSecondsSinceEpoch() const
{
	return m_microSecondsSinceEpoch;
}

inline time_t Timestamp::secondsSinceEpoch() const
{
	return m_microSecondsSinceEpoch / kMicroSecondsPerSecond;
}

