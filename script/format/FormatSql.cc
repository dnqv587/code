#include "FormatSql.h"

#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <vector>

FormatSql::FormatSql(std::string source, std::string obj)
	:_in(source),
	_out(obj)
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
		else if (*in_iter >= 'A' && *in_iter <= 'Z')
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
    if (argc == 2)
	{
		FormatSql fm(argv[1], "FormatSql.txt");
		ret=fm.format();
	}
	else if (argc == 3)
	{
		FormatSql fm(argv[1], argv[2]);
		ret=fm.format();
	}
	else
	{
		FormatSql fm("sql.txt", "FormatSql.txt");
		ret=fm.format();
	}
    if(ret)
    {
        std::cout << "Format SQL Success!" << std::endl;
    }

#ifdef _WIN32
        system("pause");
#endif 
    
	return 0;
}