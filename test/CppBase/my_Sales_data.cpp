#include <iostream>
#include <string>
#include "my_Sales_data.h"
#include <time.h>

using namespace std;

void my_Sales_data::add(const struct book& temp)  
{
	book* lib_book = nullptr;
	if (this->is_exist(temp.book_no, lib_book))
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
	book* lib_book = nullptr;
	if (is_exist(temp.book_no, lib_book))
	{
		//library.erase(lib_book);
	}
	else
	{
		library.push_back(temp);
	}
}

void my_Sales_data::print_lib() const
{
	for (auto iter = library.begin(); iter != library.end(); ++iter)
	{
		cout << "编号：" << iter->book_no << endl;
		cout << "书名：" << iter->book_name << endl;
		cout << "数量：" << iter->book_num << endl;
		cout << "价格：" << iter->book_price << endl;
		cout << "日期：" << iter->book_time << endl;

	}
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
