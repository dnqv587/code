#pragma once
#include "../base/copyable.h"
#include "../base/Type.h"
#include <sys/time.h>
#include <string>

/*
* ʱ����
* ��1970-1-1������΢��---UTC
*/
class Timestamp :public copyable
{
public:
	Timestamp();

	Timestamp(int64_t microSecondsSinceEpoch);

	bool operator>(const Timestamp& that);

	bool operator<(const Timestamp& that);

	bool operator== (const Timestamp & that);
	//���ؾ�ȷʱ���Timestamp��
	static Timestamp now();
	//��time_t��ʽ��ΪTimestamp��
	static Timestamp fromUnixTime(time_t time, int microSecond);
	static Timestamp fromUnixTime(time_t time);
	//����
	void swap(Timestamp& that);
	//ת��Ϊ�ַ���
	//��ʽ��<seconds>.<microseconds>
	std::string toString()const;
	//��ʽ��ʱ���ַ���
	// showMicroseconds:�Ƿ���΢����㣬isLocal�Ƿ���ʹ�ñ���ʱ��������ΪĬ��ʱ��
	std::string formatString(bool showMicroseconds = true, bool isLocal = false) const;
	//������1970-1-1������΢��
	int64_t microSecondsSinceEpoch() const;
	//������1970-1-1��������
	time_t secondsSinceEpoch() const;
private:
	int64_t m_microSecondsSinceEpoch;//΢��
};

