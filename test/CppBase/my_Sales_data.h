#ifndef __MY_SALES_DATA_H__
#define __MY_SALES_DATA_H__

#include <string>
#include <vector>
#include <time.h>

struct book
{
	unsigned int book_no;
	std::string book_name;
	size_t book_num;
	float book_price;
	std::string book_time;
};

class my_Sales_data
{
	friend void library();//友元
public:
	my_Sales_data() = default;//默认构造函数
	my_Sales_data(const book&);
	~my_Sales_data();
	//添加
	void add(const struct book&);
	//去除
	void sub(const struct book&);
	//列表
	void print_lib() const;
	//书是否存在
	bool is_exist(const unsigned int & ,  book** , std::vector<book>::iterator **);

private:
	std::vector <book> library;//书籍的信息
	size_t all_num;//书籍的总和
	double revenue;//收入
};

//void library();
#endif
