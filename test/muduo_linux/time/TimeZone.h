#pragma once
#include "../base/copyable.h"
#include <memory>
#include <time.h>

class TimeZone:public copyable
{
public:
	TimeZone(int UTC, const char* tzname);
	//�Ƿ���Ч
	bool valid() const
	{
		return !!m_data;
	}
	//��time_tʱ��ת��Ϊtm�ṹʱ��
	struct tm toLocalTime(time_t sec) const;




	struct Data;
private:
	std::shared_ptr< TimeZone::Data>m_data;
};
