#pragma once
#include <string>
using namespace std;

class Codec
{
public:
	Codec() = default;
	virtual ~Codec() {}//����������->��̬

	virtual string encodeMsg();
	virtual void* decodeMsg();
};
