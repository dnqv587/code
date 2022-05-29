#pragma once
#include <sys/types.h>
#include <string>

namespace ProcessInfo
{
	//进程ID
	pid_t pid();
	std::string pidString();

	//主机名
	std::string hostname();


}