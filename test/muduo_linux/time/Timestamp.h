#pragma once
#include "../base/copyable.h"
#include "../base/Type.h"

class Timestamp :public copyable
{
public:
	Timestamp();

private:
	int64_t m_microSecondsSinceEpoch;
};

