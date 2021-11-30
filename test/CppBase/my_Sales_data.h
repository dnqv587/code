#ifndef __MY_SALES_DATA_H__
#define __MY_SALES_DATA_H__

#include <string>
#include <vector>
#include <time.h>

struct book
{
	unsigned int book_no;
	string book_name;
	size_t book_num;
	float book_price;
	string book_time;
};

class my_Sales_data
{
public:
	//添加
	void add(const struct book&);
	//去除
	void sub(const struct book&);
	//列表
	void print_lib(const my_Sales_data&) const;
	//书是否存在
	bool is_exist(const unsigned int & no, book* temp);

private:
	vector<book>library;//书籍的信息
	size_t all_num;//书籍的总和
	double revenue;//收入
};

#endif
