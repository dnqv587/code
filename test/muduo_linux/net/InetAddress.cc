#include "InetAddress.h"
#include <stddef.h>

InetAddress::InetAddress(uint8_t port, bool loopbackOnly, bool ipv6)
{
	static_assert(offsetof(InetAddress, _addr) == 0, "");
}
