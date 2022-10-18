//
// Created by dainq45972 on 2022/10/18.
//

#include "geneNum.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>

geneNum::geneNum(std::string filename)
	:_out(filename)
{

}

geneNum::~geneNum()
{
	_out.close();
}

void geneNum::generate(int64_t begin,int64_t end,int n)
{
	char str[1024];
	char num[32];
	sprintf(num,"%%0%dd",n);
	for(int i=begin;i<=end;++i)
	{
		memset(str,0x00,sizeof(str));
		sprintf(str,num,i);
		_out<<str;
		if(i!=end)
		{
			_out<<',';
		}
	}
	_out.flush();
}

int main(int argc,char** argv)
{
	std::string file="geneNum.txt";
	int begin=0,end=0,n=1;
	for(int i=0;i<argc;++i)
	{
		if(!std::string(argv[i]).compare("-o"))
		{
			file = std::string(argv[i + 1]);
		}
		else if(!std::string(argv[i]).compare("-b"))
		{
			begin= ::atoi(argv[i + 1]);
		}
		else if(!std::string(argv[i]).compare("-e"))
		{
			end = ::atoi(argv[i + 1]);
		}
		else if(!std::string(argv[i]).compare("-n"))
		{
			n = ::atoi(argv[i + 1]);
		}
	}
	geneNum gen(file);
	gen.generate(begin,end,n);


#ifdef _WIN32
	system("pause");
#endif
	return 0;
}
