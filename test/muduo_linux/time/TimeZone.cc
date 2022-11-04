#include "TimeZone.h"
#include "../base/Type.h"
#include <vector>
#include <string>
#include <assert.h>

namespace detail
{
	// 记录GMT时间和本地时间的转化信息基本单元
	struct Transition
	{
		time_t gmttime;//GMT时间
		time_t localtime;//本地地区时间
		int localtimeIdx;// Data.localtimes里对应的本地时区时间索引

		Transition(time_t t, time_t l, int localIdx)
			: gmttime(t), localtime(l), localtimeIdx(localIdx)
		{ }

	};

	// 记录本地时区时间信息基本单元
	struct Localtime
	{
		time_t gmtOffset;// GMT时间转化为本地时间的差值
		bool isDst;// 夏令时标记
		int arrbIdx;// Data.names对应的时区名名索引

		Localtime(time_t offset, bool dst, int arrb)
			: gmtOffset(offset), isDst(dst), arrbIdx(arrb)
		{ }
	};
}

// 时区数据管理类,主要用于管理GMT时间和本地时区时间的转化
// 协调世界时UTC不与任何地区位置相关，也不代表此刻某地的时间，所以在说明某地时间时要加上时区
// 也就是说GMT并不等于UTC，而是等于UTC+0，只是格林尼治刚好在0时区上。
struct TimeZone::Data
{
	std::vector<detail::Transition> transitions;
	std::vector<detail::Localtime> localtimes;
	std::vector<std::string> names;
	std::string abbreviation;
};

TimeZone::TimeZone(int UTC, const char* tzname)
	:m_data(new TimeZone::Data)
{
	m_data->localtimes.push_back(detail::Localtime(UTC, false, 0));
	m_data->abbreviation = tzname;

}

struct tm TimeZone::toLocalTime(time_t sec) const
{
	struct tm localTime;
	memZero(&localTime, sizeof(localTime));
	assert(this->m_data != NULL);
}

