#include "memPool.h"
#include <stdint.h>
#include <list>
#include <map>
#include <vector>
#pragma warning(disable:4996) 

std::string format(uint64_t num)
{
	std::list<char> ret ;
	ret.push_front('元');
	std::string ch = "零壹贰叁肆伍陆柒捌玖";
	std::string re = "拾佰仟万亿";
	int count = 0;
	int zeroCount = 0;
	bool flag = false;

	while (num)
	{
		int i = num % 10;
		num /= 10;
		if (i == 0)
		{
			if (!flag && count)
			{
				ret.push_front(ch[i]);
			}
			flag = true;
			++zeroCount;
			++count;
			if (zeroCount == 5)
				ret.push_front(re[3]);
			else if (zeroCount == 9)
				ret.push_front(re[4]);
		}
		else
		{
			zeroCount = 0;
			flag = false;
			if (count > 0 && count<4)
			{
				ret.push_front(re[count - 1]);
			}
			else if (count >= 4 && count <8)
			{
				if (count == 4)
					ret.push_front(re[3]);
				else
					ret.push_front(re[count - 5]);
			}
			else if (count >=8 )
			{
				if (count == 8)
					ret.push_front(re[4]);
				else
					ret.push_front(re[count - 9 - ((count - 9) / 5) * 5]);
			}

			ret.push_front(ch[i]);
			++count;
		}
	}
	std::string res;
	for (auto c : ret)
	{
		res.push_back(c);
	}
	return res;
}
 
void VarInit(unsigned char* pucArg)
{
	*pucArg = 1;
	return;
}
void Test()
{
	unsigned char ulGlobal;
	VarInit((unsigned char*)&ulGlobal);
	printf("%lu", ulGlobal);
	return;
}

class Account
{
public:
	Account(const char* id)
	{
		printf("A");
	}

	Account(const Account& a)
	{
		printf("B");
	}

	Account& operator= (const Account& a)
	{
		printf("C");
		return *this;
	}
private:
	std::string m_id;
};

void addDate(char* p)
{
	int year = 11111111111111111;
	int day = 2;
	if (p != NULL)
	{
		sprintf(p, "%s-%d-%d", "Channel", year, day);
	}
}

void func()
{
int a = 10;
int* p = &a;
printf("%d\n",*p++);
int* k = p++;
printf("%d\n",k - p);
printf("%d\n",(int)k - (int)p);
int* l = p + 1;
printf("%d\n",l - p);
int* m = ++p;
printf("%d\n",m - p);
}

std::vector<std::vector<int>> turnMatrix(int n)
{
	std::vector<std::vector<int>> matrix(n);
	for (auto& s : matrix)
	{
		s.resize(n);
	}
	float ring = ceil((float)n / 2.00);
	int begin = 0, end = n-1, num = 1;

	while (ring--)
	{
		for (int i = begin; i <= end; ++i)
		{
			matrix[begin][i] = num;
			++num;
		}

		for (int i = begin + 1; i <= end; ++i)
		{
			matrix[i][end] = num;
			++num;
		}

		for (int i = end-1; i >=begin; --i)
		{
			matrix[end][i] = num;
			++num;
		}

		for (int i = end - 1; i > begin; --i)
		{
			matrix[i][begin] = num;
			++num;
		}

		--end;
		++begin;
	}

	for(auto i:matrix)
	{
		for (auto j : i)
		{
			std::cout << j <<" ";
		}
		std::cout << std::endl;
	}

	return matrix;
}

std::string alphabet(std::string str)
{
	std::string ret;
	std::string captal;
	for (auto c : str)
	{
		if (c >= 'A' && c <= 'Z')
		{
			captal.push_back(c);
		}
		else
		{
			ret.push_back(c);
		}
	}
	ret.append(captal);
	return ret;
}

int main()
{
	//Test();
	//std::cout << format(231724) << std::endl;


	//Account a = "12315";
	//Account b = a;
	//Account c(a);
	//return 0;

	//turnMatrix(5);
	std::cout<<alphabet("Alps-Electronic")<<std::endl;
}