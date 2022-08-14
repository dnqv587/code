#pragma once
#include "../base/copyable.h"
#include "../base/Type.h"
#include <sys/time.h>
#include <string>
#include <assert.h>
#include <concepts>

constexpr int kMicroSecondsPerSecond = 1000 * 1000;//每秒中微秒单位

struct DateTime
{
	int Year;//年
	int mon;//月
	int day;//日
	int hour;//时
	int min;//分
	int sec;//秒
	int msec;//微秒

	struct tm tm;

	DateTime()
		:Year(0), mon(0), day(0), hour(0), min(0), sec(0), msec(0), tm{0}
	{
	}
	DateTime(int Year, int mon, int day, int hour, int min, int sec, int msec, struct tm tm)
		:Year(Year), mon(mon), day(day), hour(hour), min(min), sec(sec), msec(msec), tm(tm)
	{
		assert(mon > 0 && mon <= 12);
		assert(day > 0 && day <= 31);
		assert(hour >= 0 && hour < 24);
		assert(min >= 0 && min < 60);
		assert(sec >= 0 && sec < 60);
		assert(msec >= 0 && msec < kMicroSecondsPerSecond);
	}

};
/*
* 时间类
* 自1970-1-1以来的微秒---UTC
*/
class Timestamp :public copyable
				//public std::equality_comparable<Timestamp>,
				//public std::less_than_comparable<Timestamp>
{
public:
	Timestamp();

	Timestamp(int64_t microSecondsSinceEpoch);

	bool operator>(const Timestamp& that);

	bool operator<(const Timestamp& that);

	bool operator== (const Timestamp & that);

	Timestamp operator+(const double seconds);

	//返回精确时间的Timestamp类
	static Timestamp now();
	//将time_t格式化为Timestamp类
	static Timestamp fromUnixTime(time_t time, int microSecond)
	{
		return Timestamp(time * kMicroSecondsPerSecond + microSecond);
	}
	static Timestamp fromUnixTime(time_t time)
	{
		return fromUnixTime(time, 0);
	}
	//交换
	void swap(Timestamp& that) 
	{
		std::swap(this->m_microSecondsSinceEpoch, that.m_microSecondsSinceEpoch);
	}
	//转换为DateTime
	DateTime toDateTime(bool isLocal = false) const;
	//转换为字符串
	//格式：<seconds>.<microseconds>
	std::string toString()const;
	//格式化时间字符串
	// showMicroseconds:是否以微秒计算，isLocal是否是使用本地时区，否则为默认时区
	std::string formatString(bool showMicroseconds = true, bool isLocal = false) const;
	//返回自1970-1-1以来的微秒
	int64_t microSecondsSinceEpoch() const
	{
		return m_microSecondsSinceEpoch;
	}
	//返回自1970-1-1以来秒数
	time_t secondsSinceEpoch() const
	{
		return m_microSecondsSinceEpoch / kMicroSecondsPerSecond;
	}
	//增加时间(秒)
	Timestamp addTime(const double seconds)
	{
		return Timestamp(m_microSecondsSinceEpoch + seconds * kMicroSecondsPerSecond);
	}
private:
	int64_t m_microSecondsSinceEpoch;//微秒
};

