#include "InetAddress.h"
#include <stddef.h>

InetAddress::InetAddress(uint8_t port, bool loopbackOnly, bool ipv6)
{
	static_assert(offsetof(InetAddress, _addr) == 0, "_addr offset 0");
	static_assert(offsetof(InetAddress, _addr6) == 0, "_addr6 offset 0");

	if (ipv6)
	{

	}
	else
	{

	}
}
