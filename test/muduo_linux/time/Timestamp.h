#pragma once
#include "../base/copyable.h"
#include "../base/Type.h"
#include <sys/time.h>
#include <string>

/*
* 时间类
* 自1970-1-1以来的微秒---UTC
*/
class Timestamp :public copyable
{
public:
	Timestamp();

	Timestamp(int64_t microSecondsSinceEpoch);

	bool operator>(const Timestamp& that);

	bool operator<(const Timestamp& that);

	bool operator== (const Timestamp & that);
	//返回精确时间的Timestamp类
	static Timestamp now();
	//将time_t格式化为Timestamp类
	static Timestamp fromUnixTime(time_t time, int microSecond);
	static Timestamp fromUnixTime(time_t time);
	//交换
	void swap(Timestamp& that);
	//转换为字符串
	//格式：<seconds>.<microseconds>
	std::string toString()const;
	//格式化时间字符串
	// showMicroseconds:是否以微秒计算，isLocal是否是使用本地时区，否则为默认时区
	std::string formatString(bool showMicroseconds = true, bool isLocal = false) const;
	//返回自1970-1-1以来的微秒
	int64_t microSecondsSinceEpoch() const;
	//返回自1970-1-1以来秒数
	time_t secondsSinceEpoch() const;
private:
	int64_t m_microSecondsSinceEpoch;//微秒
};

