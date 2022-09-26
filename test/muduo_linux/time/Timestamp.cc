#include "Timestamp.h"
#include <utility>




 Timestamp::Timestamp()
	:m_microSecondsSinceEpoch(0)
{

}

 Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
	:m_microSecondsSinceEpoch(microSecondsSinceEpoch)
{

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


DateTime Timestamp::toDateTime(bool isLocal /*= false*/) const
{
	DateTime time;
	time.msec = m_microSecondsSinceEpoch % kMicroSecondsPerSecond;
	int64_t sec = m_microSecondsSinceEpoch / kMicroSecondsPerSecond;
	struct tm t;
	if (isLocal)
	{
		::localtime_r(&sec, &t);
	}
	else
	{
		::gmtime_r(&sec, &t);
	}
	time.Year = t.tm_year;
	time.mon = t.tm_mon;
	time.day = t.tm_mday;
	time.hour = t.tm_hour;
	time.min = t.tm_min;
	time.sec = t.tm_sec;
	time.tm = t;
	return time;
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


