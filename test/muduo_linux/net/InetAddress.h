#pragma once
#include "../base/copyable.h"
#include <netinet/in.h>

class InetAddress :copyable
{
public:
	explicit InetAddress(uint8_t port, )

private:
	union
	{
		sockaddr_in addr;
		sockaddr_in6 addr6;
	} m_addr;

};
