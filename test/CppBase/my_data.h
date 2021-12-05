#ifndef __MY_DATA_H__
#define __MY_DATA_H__

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
	void print_lib() const; //const修改隐式this指针类型,该成员函数不可修改this所指的对象
	//书是否存在
	bool is_exist(const unsigned int & , book** , std::vector<book>::iterator **);

private:
	std::vector <book> library;//书籍的信息
	mutable size_t all_num;//书籍的总和;mutable表示可变数据成员
	double revenue;//收入
};


class Screen
{
public:
	typedef std::string::size_type  pos;

	Screen() = default;
	//Screen(int width, int height);

	Screen(pos ht, pos wd, char c) :height(ht), width(wd), content(ht*wd, c) {};

	char get() const { return content[cursor]; }

	inline char get(pos , pos ) const;

	Screen &move(pos, pos);

	Screen &set(char);
	Screen &set(pos, pos, char);

private:

	pos cursor = 0;

	pos height = 0, width = 0;

	std::string content{'1'};

	mutable size_t access_ctr;


};


#endif
