#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>

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

int main(int argc, char** argv)
{
	//vector_half();
	//scores();
	//array_init_vector();
	system("pause");
	return 0;
}