#include <iostream>
#include <string>
#include <time.h>

#include "my_data.h"

using namespace std;

my_Sales_data::my_Sales_data(const book &temp) : all_num(0), revenue(0)
{
	library.push_back(temp);

}

my_Sales_data::~my_Sales_data()
{

}

void my_Sales_data::add(const struct book& temp)  
{
	book *lib_book = nullptr;
	vector<book>::iterator *it = nullptr;
	if (this->is_exist(temp.book_no, &lib_book,&it))//使用地址传递，传递指针的地址
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
	book *lib_book = nullptr;
	vector<book>::iterator *it = nullptr;
	if (is_exist(temp.book_no, &lib_book,&it))
	{
		library.erase(*it);
	}
	else
	{
		library.push_back(temp);
	}
}

void my_Sales_data::print_lib() const
{
	system("cls");
	for (auto iter = library.begin(); iter != library.end(); ++iter)
	{
		cout << endl;
		cout << "编号：" << iter->book_no << endl;
		cout << "书名：" << iter->book_name << endl;
		cout << "数量：" << iter->book_num << endl;
		cout << "价格：" << iter->book_price << endl;
		cout << "日期：" << iter->book_time << endl;

	}
	all_num++;
}

bool my_Sales_data::is_exist( const unsigned int & no, book** temp, vector<book>::iterator **it)
{
	for (auto iter = this->library.begin(); iter != this->library.end(); ++iter)
	{
		if (iter->book_no == no)
		{
			*temp = &(*iter);
			*it = &iter;
			return true;
		}
	}
	return false;
}
//---------------Screen


char Screen::get(pos r, pos c) const
{
	pos row = r * width;
	return content[row + c];

}

inline Screen & Screen::move(pos r, pos c)
{
	pos row = r * width;
	cursor = row + c;
	return *this;
}

Screen & Screen::set(char c)
{
	content[cursor] = c;
	return *this;
}

Screen & Screen::set(pos r , pos col, char ch)
{
	content[r*width + col] = ch;
	return *this;
}

