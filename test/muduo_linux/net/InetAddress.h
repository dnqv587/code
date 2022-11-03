#pragma once
#include "../base/copyable.h"
#include <netinet/in.h>


class InetAddress :copyable
{
public:
	InetAddress(uint8_t port, bool loopbackOnly = false, bool ipv6 = false);

private:

	union 
	{
		sockaddr_in _addr;
		sockaddr_in6 _addr6;
	};
};