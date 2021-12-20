#pragma once
#include <string>
using namespace std;

class Codec
{
public:
	Codec() = default;
	virtual ~Codec() {}//–ÈŒˆππ∫Ø ˝->∂‡Ã¨

	virtual string encodeMsg();
	virtual void* decodeMsg();
};
