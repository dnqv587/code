#include "TimeZone.h"
#include "../base/Type.h"
#include <vector>
#include <string>
#include <assert.h>

namespace detail
{
	// ��¼GMTʱ��ͱ���ʱ���ת����Ϣ������Ԫ
	struct Transition
	{
		time_t gmttime;//GMTʱ��
		time_t localtime;//���ص���ʱ��
		int localtimeIdx;// Data.localtimes���Ӧ�ı���ʱ��ʱ������

		Transition(time_t t, time_t l, int localIdx)
			: gmttime(t), localtime(l), localtimeIdx(localIdx)
		{ }

	};

	// ��¼����ʱ��ʱ����Ϣ������Ԫ
	struct Localtime
	{
		time_t gmtOffset;// GMTʱ��ת��Ϊ����ʱ��Ĳ�ֵ
		bool isDst;// ����ʱ���
		int arrbIdx;// Data.names��Ӧ��ʱ����������

		Localtime(time_t offset, bool dst, int arrb)
			: gmtOffset(offset), isDst(dst), arrbIdx(arrb)
		{ }
	};
}

// ʱ�����ݹ�����,��Ҫ���ڹ���GMTʱ��ͱ���ʱ��ʱ���ת��
// Э������ʱUTC�����κε���λ����أ�Ҳ������˿�ĳ�ص�ʱ�䣬������˵��ĳ��ʱ��ʱҪ����ʱ��
// Ҳ����˵GMT��������UTC�����ǵ���UTC+0��ֻ�Ǹ������θպ���0ʱ���ϡ�
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

