#include "ProcessInfo.h"
#include <unistd.h>

inline pid_t ProcessInfo::pid()
{
	return ::getpid();
}

std::string ProcessInfo::pidString()
{
	return std::to_string(ProcessInfo::pid());
}

std::string ProcessInfo::hostname()
{
	// HOST_NAME_MAX 64
	// _POSIX_HOST_NAME_MAX 255
	char buf[256];
	if (::gethostname(buf, sizeof buf) == 0)
	{
		buf[sizeof(buf) - 1] = '\0';
		return buf;
	}
	else
	{
		return "unknowhost";
	}
}

