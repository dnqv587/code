#pragma once
#include <string>
#include <vector>

namespace Tools
{
	namespace String
	{
		bool Contain(std::string& str, char c);

		std::vector<std::string> Split(std::string src, char dat);

		void Trim(std::string& str);
	}

}
