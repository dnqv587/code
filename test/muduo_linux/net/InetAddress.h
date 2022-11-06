#pragma once
#include "../base/copyable.h"
#include <netinet/in.h>
#include <string>


class InetAddress :copyable
{
public:
	explicit InetAddress(in_port_t port, bool loopbackOnly = false, bool ipv6 = false);

	explicit InetAddress(std::string addr, in_port_t port, bool ipv6 = false);

private:

	union
	{
		sockaddr_in _addr;
		sockaddr_in6 _addr6;
	};
};