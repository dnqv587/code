#pragma once
#include <sys/types.h>
#include <string>

namespace ProcessInfo
{
	//����ID
	pid_t pid();
	std::string pidString();

	//������
	std::string hostname();


}