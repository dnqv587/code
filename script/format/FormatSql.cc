#include "FormatSql.h"

#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <vector>

FormatSql::FormatSql(std::string source, std::string obj,bool tolower)
	:_in(source),
	_out(obj),
	_toLower(tolower)
{
	_in >> std::noskipws;
}

FormatSql:: ~FormatSql()
{

}

bool FormatSql::format()
{
	if (!(_in && _out))
	{
		std::cerr << "file open failt!" << std::endl;
		return false;
	}
	std::string line;
	std::istream_iterator<char> in_iter(_in);
	std::istream_iterator<char> eof;
	std::ostream_iterator<char> out_iter(_out);
	std::vector<char> res;
	bool flag = false;
	while (in_iter != eof)
	{
		if (*in_iter == ' ' || *in_iter == '\r' || *in_iter == '\n' || *in_iter == '\t')
		{
			if (!flag)
			{
				res.push_back(' ');
				flag = true;
			}

			
		}
		else if (_toLower && *in_iter >= 'A' && *in_iter <= 'Z')
		{
			flag = false;
			res.push_back(*in_iter + 32);
		}
		else
		{
			res.push_back(*in_iter);
			flag = false;
		}
		++in_iter;
	}
	res.push_back('\0');
    res.push_back('\n');
	std::copy(res.cbegin(), res.cend(), out_iter);
    return true;
}

int main(int argc, char* argv[])
{
    bool ret = false;
	std::string in("sql.txt"), out("FormatSql.txt");
	bool tolower = false;

	for (int i = 0; i < argc;++i)
	{
		if(!std::string(argv[i]).compare("-i"))
		{
			in = argv[i + 1];
		}
		else if(!std::string(argv[i]).compare("-o"))
		{
			out = argv[i + 1];
		}
		else if(!std::string(argv[i]).compare("-l"))
		{
			tolower = true;
		}
	}
	FormatSql fm(in, out, tolower);
	ret=fm.format();
	if (ret)
	{
		std::cout << "Format SQL Success!" << std::endl;
	}

#ifdef _WIN32
        system("pause");
#endif 
    
	return 0;
}