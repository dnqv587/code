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
#include <stack>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <set>




class Folder;
class Massage
{
	friend class Folder;
	friend void swap(Massage& lhs, Massage& rhs);
public:
	explicit Massage(const std::string& msg = "") :content(msg)//有参构造
	{
		//addToFolder(*this);
	}

	Massage(const Massage& msg) :content(msg.content)//拷贝构造
	{
		addToFolder(msg);//将本消息添加到指定的Folder中
	}

	Massage& operator=(const Massage& rhs)
	{
		removeFromFolder();
		this->content = rhs.content;
		this->folders = rhs.folders;
		addToFolder(rhs);
		return *this;

	}

	~Massage()
	{
		removeFromFolder();
	}

	void save(Folder& fld)
	{
		folders.insert(&fld);
		fld.addMsg(this);
	}

	void remove(Folder& fld)
	{
		folders.erase(&fld);
		fld.remMsg(this);
	}

	void addFolder(Folder* fld)
	{
		folders.insert(fld);
	}

	void removeFolder(Folder* fld)
	{
		folders.erase(fld);
	}

private:
	//将本Massage添加到指向参数的Folder中
	void addToFolder(const Massage& msg)
	{
		for (auto i : msg.folders)
		{
			i->addMsg(this);
		}
	}

	//将folders中的每个Folder删除本Massage
	void  removeFromFolder()
	{
		for (auto i : folders)
		{
			i->remMsg(this);
		}
	}

private:
	std::string content;//实际内容

	std::set<Folder*> folders;//包含本Massage的Folder

};


class Folder
{
public:
	Folder() = default;

	Folder(const Folder& fld)
	{
		addToMsg(fld);
	}

	Folder& operator=(const Folder& fld)
	{
		removeFromMsg();
		this->massages = fld.massages;
		addToMsg(fld);
	}
	~Folder()
	{
		removeFromMsg();
	}

	void addMsg(Massage* msg)
	{
		massages.insert(msg);
	}

	void remMsg(Massage* msg)
	{
		massages.erase(msg);
	}

private:
	void addToMsg(const Folder& fld)
	{
		for (auto i : fld.massages)
		{
			i->addFolder((Folder*)&fld);
		}
	}

	void removeFromMsg()
	{
		for (auto i : massages)
		{
			i->removeFolder(this);
		}
	}

private:
	std::set<Massage*>massages;

};

void swap(Massage& lhs, Massage& rhs)
{
	using std::swap;

	for (auto i : lhs.folders)
	{
		i->remMsg(&lhs);
	}

	for (auto i : rhs.folders)
	{
		i->remMsg(&rhs);
	}

	swap(lhs.folders, rhs.folders);
	swap(lhs.content, rhs.content);

	for (auto i : lhs.folders)
	{
		i->addMsg(&lhs);
	}

	for (auto i : rhs.folders)
	{
		i->addMsg(&rhs);
	}

}






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
