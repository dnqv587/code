#include "SqlTools.h"

#include <iterator>
#include <fstream>
#include <sstream>
#include <algorithm>


SqlTools::SqlTools(std::string source, std::string obj)
	:_in(source),
	_out(obj)
{
	
}

SqlTools:: ~SqlTools()
{
	_in.close();
	_out.close();
}

void SqlTools::noskipws()
{
	_in >> std::noskipws;
}
