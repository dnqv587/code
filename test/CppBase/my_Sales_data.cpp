#include <iostream>
#include <string>
#include "my_Sales_data.h"
#include <time.h>

using namespace std;

void my_Sales_data::add(const struct book& temp)  
{
	
	cout << "编号：";
	cin >> temp.book_no;
	cout << endl << "书名：";
	cin > temp.book_name;
	cout << endl << "数量：";
	cin >> temp.book_num;
	cout << endl << "价格：";
	cin >> temp.book_price;

	time_t curtime;
	time(&curtime);
	temp.book_time = ctime(&curtime);

	book *lib_book;
	if (is_exist(temp.book_no, lib_book))
	{
		lib_book->book_name = temp.book_name;
		lib_book->book_num += temp.book_num;
		lib_book->book_price = temp.book_price;
		lib_book->book_time = temp.book_time;
	}
	else
	{
		library.push_back(temp);
	}

}

void my_Sales_data::sub(const struct book& temp)
{

}

void my_Sales_data::print_lib(const my_Sales_data&) const
{

}

bool my_Sales_data::is_exist( const unsigned int & no,book* temp)
{
	for (auto iter = this->library.begin(); iter != this->library.end(); ++iter)
	{
		if (iter->book_no == no)
		{
			temp = &(*iter);
			return true;
		}
	}
	return false;
}
