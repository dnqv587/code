#include <iostream>
#include <string>

#include "../person.pb.h"

using namespace std;
using namespace people;
 
int main()
{
	//1.创建person对象，并初始化
	person p;
	p.set_id(1001);
	p.add_name();//申请一块内存，存储名字
	p.set_name(0,"路飞");
	p.add_name();//申请一块内存，存储名字
	p.set_name(1, "杰");
	p.set_sex("male");
	p.set_age(21);
	p.set_clolor(blue);

	infomation::Info* inf = p.mutable_info();//inf指向对象p下的info类
	inf->set_address("天安门");
	inf->set_number(911);
	  
	//2.将person对象序列化->字符串
	string output;
	p.SerializeToString(&output);
	cout << "序列化后的数据:" << output << endl;;
	//3.数据传输
	
	//4.接受数据，解析->解码->原始数据
	//4.1 创建person对象
	person pp;
	pp.ParseFromString(output);
	
	//5.处理原始数据->打印数据信息
	cout << "id:" << pp.id() << "  name:" << pp.name(1) <<p.name(0)<< "  sex:" << pp.sex() << "  age:" << pp.age() <<"  clolor:"<<pp.clolor()
		<<"  Info:"<<p.info().address()<<p.info().number()
		<< endl;

	return 0;
}