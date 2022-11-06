#include "InetAddress.h"
#include "Socket.h"
#include <stddef.h>
#include "../base/Type.h"


InetAddress::InetAddress(in_port_t port, bool loopbackOnly, bool ipv6)
{
	static_assert(offsetof(InetAddress, _addr) == 0, "_addr offset 0");
	static_assert(offsetof(InetAddress, _addr6) == 0, "_addr6 offset 0");

	if (ipv6)
	{
		memZero(&_addr6, sizeof _addr6);
		_addr6.sin6_family = AF_INET6;
		_addr6.sin6_port = Endian::HostToNetwork16(port);
		_addr6.sin6_addr = loopbackOnly ? in6addr_loopback : in6addr_any;
	}
	else
	{
		memZero(&_addr, sizeof _addr);
		_addr.sin_family = AF_INET;
		_addr.sin_port = Endian::HostToNetwork16(port);
		_addr.sin_addr.s_addr = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
	}
}

InetAddress::InetAddress(std::string addr, in_port_t port, bool ipv6)
{

}



