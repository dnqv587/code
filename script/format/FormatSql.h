#pragma once
#include <string>
#include <fstream>

class FormatSql
{
public:
	FormatSql(std::string source, std::string obj);

	~FormatSql();

	bool format();

private:
	std::ifstream _in;
	std::ofstream _out;
};