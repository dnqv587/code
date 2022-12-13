#include "Tools.h"
#include <string.h>

bool Tools::String::Contain(std::string& str, char c)
{
	return !!::strchr(str.c_str(), c);
}

std::vector<std::string> Tools::String::Split(std::string src, char dat)
{

	if (src.empty() || !Contain(src, dat))
	{
		return std::vector<std::string>();
	}
	std::vector<std::string> resault;
	std::string::const_iterator begin = src.cbegin();
	for (auto iter= src.cbegin();iter!=src.cend();++iter)
	{
		if (*iter == dat)
		{
			//std::string
			resault.emplace_back(std::string(begin, iter));
			begin = iter + 1;
		}
	}
	resault.emplace_back(std::string(begin, src.cend()));
	return resault;
}

void Tools::String::Trim(std::string& str)
{
	std::string::const_iterator begin;
	std::string::const_reverse_iterator end;

	for (std::string::const_iterator iter = str.cbegin(); iter != str.cend(); ++iter)
	{
		if (*iter != ' ')
		{
			begin = iter;
			break;
		}
	}
	if (begin != str.cbegin())
	{
		str.erase(str.begin(), begin);
	}
	for (std::string::const_reverse_iterator riter = str.crbegin(); riter != str.crend() ; ++riter)
	{
		if (*riter != ' ')
		{
			end = riter;
			break;
		}
	}
	if (end != str.crbegin())
	{
		str.erase(end.base(), str.end());
	}
}
