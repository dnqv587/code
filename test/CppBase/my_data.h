#ifndef __MY_DATA_H__
#define __MY_DATA_H__

#include <time.h>
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <deque>
#include <ctype.h>
#include <exception>
#include <windows.h>
#include <assert.h>
#include<fstream>
#include <sstream>



struct personInfo
{
	std::string name;
	std::vector<std::string> phone;
};

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
	explicit my_Sales_data(const book&);
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

class Debug 
{
public:
	constexpr Debug(bool all=true):hw(all),io(all),other(all){ }

	constexpr Debug(bool h,bool i,bool o) : hw(h), io(i), other(o) { }
	
	constexpr bool any() { return hw || io || other; }

	void set_hw(bool h) { hw = h; }

	void set_io(bool i) { io = i; }
	
	void set_other(bool o) { other = o; }

private:
	bool hw;    //硬件错误
	bool io;	//io错误	
	bool other; //其他错误
};

class Account
{
public:
	Account(std::string own,double amo):owner(own),amount(amo) { }

	double getAmount() { return amount; }

	void calculate() { amount += amount * interestRate; }

	static double rate() { return interestRate; }

	static void rate(double);

private:
	std::string owner;//用户
	double amount;//总数
	static double interestRate;
	constexpr static double initRate = 2.71828;//静态成员的类内初始化
};

#endif
