#pragma once
#include <string>
#include <fstream>

class SqlTools
{
public:
	SqlTools() = default;
	
	SqlTools(std::string source, std::string obj);

	virtual  ~SqlTools();

	void noskipws();

protected:
	std::ifstream _in;
	std::ofstream _out;
};