#pragma once
#include "../base/copyable.h"
#include <memory>
#include <time.h>

class TimeZone :public copyable
{
public:
	TimeZone(int UTC, const char* tzname);
	//是否有效
	bool valid() const
	{
		return !!m_data;
	}
	//将time_t时间转换为tm结构时间
	struct tm toLocalTime(time_t sec) const;




	struct Data;
private:
	std::shared_ptr< TimeZone::Data>m_data;
};
