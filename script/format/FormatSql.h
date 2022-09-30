#pragma once
#include <string>
#include <fstream>
#include <atomic>

class FormatSql
{
public:
	FormatSql(std::string source, std::string obj, bool tolower);

	~FormatSql();

	bool format();

private:
	std::ifstream _in;
	std::ofstream _out;
	std::atomic<bool> _toLower;
};