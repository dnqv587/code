#include "Tools.h"
#include <string.h>

bool Tools::String::Contain(std::string str, char c)
{
	return !!::strchr(str.c_str(), c);
}

std::vector<std::string> Tools::String::Split(std::string src, char dat)
{
	if (src.length() == 0 || !Contain(src, dat))
	{
		return std::vector<std::string>();
	}
	std::vector<std::string> resault;
	std::string::const_iterator begin = src.cbegin();
	for (auto iter= src.cbegin();iter!=src.cend();++iter)
	{
		if (*iter == dat)
		{
			resault.emplace_back(std::string(begin, iter));
			begin = iter + 1;
		}
	}
	return resault;
}
