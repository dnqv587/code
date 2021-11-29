#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <exception>
#include <windows.h>
#include <assert.h>

#include "./1/Sales_item.h"

using namespace std;
void for_and_vector()
{
	string s;
	vector <string> vs;
	vector <int>ten{ 1,2,3,4,5,6,7,8,9,10,11 };
	//unsigned i = 0;
	//for (auto c : ten)
	//{
	//	cout << c + ten[ten.size() - ++i] << endl;
	//}

	//auto iter = ss.begin();
	//while (iter != ss.end())
	//{
	//	cout << *iter++ << endl;
	//}
	//for (auto iter = ten.begin(); iter != ten.end(); iter++)
	//{
	//	*iter = *iter * 2;
	//	cout << *iter ;
	//}
	string sss;
	while (getline(cin, s))
	{
		vs.push_back(s);
		sss = "";
		if (!s.empty())
		{
			for (auto c : s)
			{
				if (!ispunct(c))
					sss += c;
			}
			cout << sss;
		}
		else
			break;
		cout << endl;
		for (auto c : vs)
		{
			for (auto &cc : c)
			{
				cc = toupper(cc);
			}
			cout << c << endl;
		}
	}
}

//二分法
void vector_half()
{
	vector <int>ten{ 1,2,3,4,5,6,7,8,9,10,11};
	int t;
	auto start = ten.begin(), end = ten.end(), mid = ten.begin() + (end - start) / 2;
	cin >> t;
	while (start != end)
	{
		if (*mid == t)
			break;
		if (t > *mid)
			start = mid + 1;
		else
			end = mid;
		mid = start + (end - start) / 2;
	}
	cout << *mid << endl;
}


//成绩分布
void scores()
{
	vector <int>count(11,0);
	int score;
	auto countSort = count.begin();
	while (cin >> score)
	{
		if (score < 0 && score>100)
			continue;
		(*(countSort + score / 10))++;
		if (score == 99)
			break;
	}

	for (auto iter = count.begin(); iter != count.end(); iter++)
	{
		cout << *iter << endl;
	}

}


//数组和vector互相初始化
void array_init_vector()
{
	int array[] = { 0,1,2,3,4,5,6,7,8,9 };
	int vect_arr[10] = {0};
	int *pt = &vect_arr[9];
	vector<int>vect(begin(array), end(array));
	for (auto c : vect)
	{
		cout << c << endl;
		*pt-- = c;
		
	}
	for (int i = 0; i < sizeof(vect_arr) / sizeof(vect_arr[0]); i++)
	{
		cout << vect_arr[i] << endl;
	}

}

//使用位运算符记录
void num_count()
{
	int num;
	unsigned long count = 0;
	unsigned long sign = 01;
	bool flag = true;

	while (cin >> num)
	{
		if (num <= 0 && num >= 31)
			continue;
		if (num == 99)
			break;
		if (num == 100)
		{
			flag = false;
			continue;
		}
		if (num == 88)
		{
			flag = true;
			continue;
		}
		if (flag)
			count |= sign << num;
		else
			count &= ~(sign << num);
	}
	printf("%X", count);
}

//异常处理
float division(float a, float b )
{
	if (b == 0)
	{
		throw "divison by zeor condition";//抛出异常，类型为const char*
	}
	return a / b;
}
void try_catch()
{
	float a, b;
	try
	{
		cin >> a;
		cin >> b;
		printf("%f\n", division(a,b));
	}
	catch (const char *msg)//抓住类型为const char* 的异常
	{
		//perror(msg);
		cerr << msg << endl;
	}
}

int static_add()
{
	static int n ;//内置类型的局部静态变量默认初始化为0
	return ++n;
}
void static_test()
{
	for (int i = 0; i < 10; i++)
	{
		cout << static_add() << endl;
	}
}

//判断是否有大写，并转换为小写
void change_to_lower(string &str)
{
	for (auto &c : str)
	{
		if (isupper(c))
		{
			c = tolower(c);
		}
	}
}
bool if_string_upper(const string &str)
{
	for (auto iter = str.begin(); iter != str.end(); iter++)
	{
		if (isupper(iter[0]))
		{
			cout << str << ":have upper" << endl;
			return true;
		}
	}
	return false;
}
void if_string_func()
{
	string str;
	while (cin >> str)
	{
		if (str == "n")
			break;
		if_string_upper(str);
		change_to_lower(str);
		cout << str << endl;
	}
}

int* pt()//返回指针类型
{
	static int a = 2;
	return &a;
}
int& ad()//返回引用类型
{
	static int b = 3;
	return b;
}

//返回数组指针
int (* get())[3] //auto get()->int(*)[3]  //尾置返回类型
{
	int s[3]{1,2,3};
	return &s;//返回数组的地址
}

//函数指针
inline int add(int a, int b){return a + b;}
inline int subtract(int a, int b){return a - b;}
inline int multiply(int a, int b){return a * b;}
inline int division(int a, int b){return a / b;}
void func_pt()
{
	//using pt = int(*)(int, int);//方法一，using别名
	//typedef int (*pt)(int, int);//方法二，typedef别名
	//int(*pt)(int, int);//方法三，函数声明

	vector<int(*)(int,int)>vec;//定义int(*)(int,int)类型vector容器
	//vector<pt)>vec;//先声明pt的类型
	vec.push_back(&add);
	vec.push_back(&subtract);
	vec.push_back(&multiply);
	vec.push_back(&division);
	for (auto iter = vec.begin(); iter != vec.end(); ++iter)
	{
		printf("%d\n", (*iter)(5, 5));
	}
}

int main(int argc, char** argv)
{
	//vector_half();
	//scores();
	//array_init_vector();
	//num_count();
	//try_catch();
	//static_test();
	//if_string_func();
	//ad() = 5;
	//printf("%d,%d", *pt(), ad());
	//printf("%d", (*get())[2]);//(*get())[2]：解引用指向数组的指针，再使用下标来获取对象的值
	func_pt();
	system("pause");
	return 0;
}