#include "my_data.h"
#include "./1/Sales_item.h"
#include "./7/Sales_data.h"

#include <initializer_list>
#include <memory>


using namespace std;
using namespace std::placeholders;

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

//使用位运算符记录
void num_count()
{
	int num;
	unsigned long count = 0;
	unsigned long sign = 01;
	bool flag = true;

	while (cin >> num)
	{
		if (num <= 0 && num >= 31)
			continue;
		if (num == 99)
			break;
		if (num == 100)
		{
			flag = false;
			continue;
		}
		if (num == 88)
		{
			flag = true;
			continue;
		}
		if (flag)
			count |= sign << num;
		else
			count &= ~(sign << num);
	}
	printf("%X", count);
}

//异常处理
float division(float a, float b )
{
	if (b == 0)
	{
		throw "divison by zeor condition";//抛出异常，类型为const char*
	}
	return a / b;
}
void try_catch()
{
	float a, b;
	try
	{
		cin >> a;
		cin >> b;
		printf("%f\n", division(a,b));
	}
	catch (const char *msg)//抓住类型为const char* 的异常
	{
		//perror(msg);
		cerr << msg << endl;
	}
}

int static_add()
{
	static int n ;//内置类型的局部静态变量默认初始化为0
	return ++n;
}
void static_test()
{
	for (int i = 0; i < 10; i++)
	{
		cout << static_add() << endl;
	}
}

//判断是否有大写，并转换为小写
void change_to_lower(string &str)
{
	for (auto &c : str)
	{
		if (isupper(c))
		{
			c = tolower(c);
		}
	}
}
bool if_string_upper(const string &str)
{
	for (auto iter = str.begin(); iter != str.end(); iter++)
	{
		if (isupper(iter[0]))
		{
			cout << str << ":have upper" << endl;
			return true;
		}
	}
	return false;
}
void if_string_func()
{
	string str;
	while (cin >> str)
	{
		if (str == "n")
			break;
		if_string_upper(str);
		change_to_lower(str);
		cout << str << endl;
	}
}

int* pt()//返回指针类型
{
	static int a = 2;
	return &a;
}
int& ad()//返回引用类型
{
	static int b = 3;
	return b;
}

//返回数组指针
int (* get())[3] //auto get()->int(*)[3]  //尾置返回类型
{
	int s[3]{1,2,3};
	return &s;//返回数组的地址
}

//函数指针
inline int add(int a, int b){return a + b;}
inline int subtract(int a, int b){return a - b;}
inline int multiply(int a, int b){return a * b;}
inline int division(int a, int b){return a / b;}
void func_pt()
{
	//using pt = int(*)(int, int);//方法一，using别名
	//typedef int (*pt)(int, int);//方法二，typedef别名
	//int(*pt)(int, int);//函数声明，未初始化
	//auto pt()->int(*)(int, int);//尾置返回类型

	vector<int(*)(int,int)>vec;//定义int(*)(int,int)类型vector容器
	//vector<pt>vec;//先声明pt的类型
	vec.push_back(&add);
	vec.push_back(&subtract);
	vec.push_back(&multiply);
	vec.push_back(&division);
	for (auto iter = vec.begin(); iter != vec.end(); ++iter)
	{
		printf("%d\n", (*iter)(5, 5));
	}
}

//salse_data类的使用
void salse_data_book()
{

	Sales_data total;//保存当前结果求和的变量
	if (read(cin, total))//读入第一笔交易
	{
		Sales_data trans;//保存下一条交易数据的变量
		while (read(cin, trans))//读入剩余的交易
		{
			if (total.isbn() == trans.isbn())//检查isbn
				total.combine(trans);//更新变量total当前的值
			else
			{
				print(cout, total) << endl;//输出结果
				total = trans;//处理下一本书
			}
		}
		print(cout, total) << endl;//输出最后一条交易
	}
	else//没有输入任何信息
	{
		cerr << "NO DATA ?" << endl;//通知用户
	}

}

//图书管理系统
void library()
{
	book inbook;
	inbook.book_no = 1;
	inbook.book_name = "C++";
	inbook.book_num = 10;
	inbook.book_price = 60;

	my_Sales_data* library = new my_Sales_data(inbook);//书库

	int mod;
	while (1)
	{
		
		cout << "添加输1" << endl << "删除输2" << endl << "输入：" ;
		cin >> mod;
		if (mod == 1)
		{
			book temp;
			cout << "编号：";
			cin >> temp.book_no;
			cout << "书名：";
			cin >> temp.book_name;
			cout << "数量：";
			cin >> temp.book_num;
			cout << "价格：";
			cin >> temp.book_price;

			time_t curtime;
			time(&curtime);

			char temp_date[26];
			ctime_s(temp_date, sizeof(temp_date), &curtime);
			temp.book_time = temp_date;

			library->add(temp);
			library->print_lib();
		}
		else if (mod == 2)
		{
			book temp;
			cout << "编号：";
			cin >> temp.book_no;
			library->sub(temp);
			library->print_lib();
		}
	}
	delete(library);
}

void bug_test()
{
	 Debug test(false, true, false);
	if (test.any())
	{
		cerr << "error message" << endl;
	}
}

double Account::interestRate = 3.14;//定义静态成员



void input(ifstream &is)
{
	string s;
	while (is >> s)
		cout << s << endl;
}

void output(ofstream &os)
{
	//string str;
	string s;
	while (cin>>s)
	{
		os << s << endl;
	}
}

//文件读写
void ifstream_8_1(int &argc, char** &argv)
{ 
	const char* path = "C:/Users/UK2021/Documents/test/abc.txt";
	string sss;
	//ifstream in(path);  //打开文件读
	//ofstream out(path);//打开文件写
	ifstream in;  //先定义
	ofstream out;
	in.open(path, ofstream::app);//后打开
	out.open(path, ofstream::out || ofstream::app);

	if (in&&out)            //如果文件打开正常
	{
		input(in);
		output(out);
	}
	else
		cerr << "couldn't open: " + string(path);//输出无法打开的文件
}

//string流的文件读写
void stringstream_test()
{
	const char* path = "C:/Users/UK2021/Documents/test/abc.txt";
	ifstream in;  //先定义
	in.open(path, ofstream::app);

	string line,word;
	vector <personInfo> people;
	while (getline(in, line))
	{
		personInfo info;//创建一个对象保存此记录对象
		istringstream record(line);//将记录绑定到读入的行
		record >> info.name;
		while (record >> word)
		{
			info.phone.push_back(word);;
		}
		people.push_back(info);


		
		for (auto iter = people.begin(); iter != people.end(); ++iter)//输出保存的信息
		{
			cout << iter->name;
			ostringstream outinfo;
			for (auto iter2 = iter->phone.begin(); iter2 != iter->phone.end(); ++iter2)
			{
				outinfo <<"||"<< *iter2;
			}
			cout << outinfo.str();
		}
		cout << endl;
	}
	in.close();

}

class ar
{
public:
	ar(int a) { b = a; }
	ar() = default;
private:
	int b;
};


void list_to_vector()
{
	list<int> a{ 1,2,3,4,5,6};
	//vector<double>b(a.cbegin(), a.cend());
	vector<double>b;
	for (auto iter = a.cbegin(); iter != a.cend(); ++iter)
	{
		b.push_back(*iter);
	}
	/*for ( auto iter=b.begin();iter!=b.end();++iter)
	{
		auto iiter = iter;

		while (iiter != b.end())
		{
			if (iiter+1 == b.end())
				break;
			swap(*iiter, *(iiter + 1));

			++iiter;
		}

	}*/
	for (const auto &c : b)
	{
		cout << c << endl;
	}

}

void swap_test()//有问题
{
	vector<string>vec;
	list<char*>lis;
	char *ss = new char[10];
	while (cin >> ss)
	{
		if (ss[0] == '9')
			break;
		lis.push_back(ss);
		for (auto iter = vec.begin(); iter != vec.end(); ++iter)
		{
			vec.assign(lis.cbegin(), lis.cend());
		}
		
	}
	for (auto &c : vec)
	{
		cout << c << endl;
	}
}

//插入测试，二分法
void insert_test()
{
	unsigned int ss;
	vector<int> order_list;

	auto start = order_list.begin(), end = order_list.end(), mid = order_list.begin() + (order_list.size()) / 2;
	while (cin >> ss)
	{
		if (ss == 999)
			break;

		start = order_list.begin(); end = order_list.end(); mid = order_list.begin() + (order_list.size()) / 2;//重置start、end、mid

		while (start != end)//二分法
		{
			if (*mid == ss)
			{
				//order_list.insert(mid, ss);
				break;
			}
			if (ss > *mid)
			{
				start = mid+1;
			}
			else
			{
				end = mid;
			}
			mid = start + (end - start) / 2;
		}

		order_list.insert(mid, ss);

		for (const auto &c : order_list)
		{
			cout << c <<"  ";
		}
		cout << endl;
	}

}

void emplace_test()
{
	Account *acc = new Account("dai",99.99);
	acc->calculate();
	deque<Account> account;

	account.emplace_back("dai", 99.99);//根据容器的类型的构造函数初始化一个对象，并压入容器中
	account.push_back(*acc);//直接将对象压入容器中

	for (auto & c : account)
	{
		cout << c.getAmount() << endl;
	}
}

void test926()
{
	int ia[] = { 0,1,1,2,3,5,8,13,21,55,89 };
	vector<int> ji(begin(ia), end(ia));
	list<int>ou(begin(ia), end(ia));

	auto jiiter = ji.begin();

	while (jiiter != ji.end())
	{
		if (!(*jiiter % 2))
		{
			jiiter = ji.erase(jiiter);//删除当前元素后，返回指向原元素的指针
		}
		else 
		{
			jiiter++;
		}
	}

	auto ouiter = ou.begin();
	while (ouiter != ou.end()) 
	{
		if (*ouiter % 2)
		{
			ouiter = ou.erase(ouiter);
		}
		else 
		{
			ouiter++;
		}
	}

	for (const auto &c : ji)
	{
		cout << c << " ";
	}
	cout << endl;

	for (const auto &c : ou)
	{
		cout << c << " ";
	}
	cout << endl;
}

void forward_list_erase_after927()
{
	forward_list<int> f_list{ 0,1,2,3,4,5,6,7,8,9 };
	auto pre = f_list.before_begin();
	auto cur = f_list.begin();
	
	while (cur != f_list.end())
	{
		if (!(*cur % 2))
		{
			cur = f_list.erase_after(pre);
		}
		else
		{
			++pre;
			//advance(pre, 1);
			++cur;
			//advance(cur, 1);
		}
	}

	for (const auto& c : f_list)
	{
		cout << c;
	}
	cout << endl;
}

void char_to_string941()
{
	vector<char> c{ 'h','e','l','l','o' };
	string s(c.begin(),c.end());

	cout << s << endl;
}

//查找并替换
void string_test946()
{
	string ss{ "bbaabbabbab" };
	string n = "ab";
	bool flag;
	int count = 0;

	for (auto iter = ss.begin(); iter != ss.end()-n.size()+1; ++iter)
	{
		auto p = iter;
		if (*iter == *n.begin() )
		{
			flag = true;
			
			for (auto iiter = n.begin()+1; iiter != n.end(); ++iiter)
			{
				if (*++p != *iiter)
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				//943
				//ss.erase(count, n.size());
				//ss.insert(iter, '2');
				//ss.insert(iter, '2');
				//944
				ss.replace(iter, iter + n.size(), "22");
			}
		}
		++count;
	}

	cout << ss << endl;

}

//分类数字字符和字母字符
void string_find_test947()
{
	string text = "ab2c3d7R4E6",numtext="0123456789";
	string num, alb;
	string::size_type pos = 0;
	while ((pos = text.find_first_of(numtext, pos)) != string::npos)
	{
		num.push_back(text[pos]);
		++pos;
	}
	pos = 0;
	for (pos = 0; (pos = text.find_first_not_of(numtext, pos)) != string::npos; ++pos)
	{
		alb.push_back(text[pos]);
	}
	cout << num << endl;
	cout << alb << endl;
}

//表达式运算----未完成
void stack_test952()
{ 
	stack<char>iner;
	stack<char>final;
	string expr = "(1+2)*3+(2+3)";
	string num;
	bool flag = false;

	for (auto iter = expr.cbegin(); iter != expr.cend(); ++iter)
	{
		flag = false;
		if (*iter == '(')
		{
			flag = true;
			iner.push(*iter);
		}
		if (*iter == ')')
		{
			flag = true;
			while (!iner.empty())
			{
				if (iner.top() != '(' && iner.top() != ')')
				{
					num.push_back(iner.top());
					iner.pop();
				}
			}
		}
	}

}

void algo_test101()
{
	vector<int> vec = { 1,2,3,4,5,6,7,8,9,1,2,1,1,2,2,3,3,4 };

	vector<double>dob = { 3.14,2.718 };

	char equal1[10] = "123456789";
	char equal2[10] = "123456789";

	cout << "count:" << count(vec.cbegin(), vec.cend(), 1) << endl;

	cout << "accumulate:" << accumulate(dob.cbegin(), dob.cend(), 0) << endl;//初始值类型为int则double元素相加和的小数会被丢弃

	cout << "equal:" << equal(begin(equal1), end(equal1), begin(equal2)) << endl;

}

void elimDups109()
{
	vector<string> svec = { "the", "quick", "red", "fox", "jumps", "over", "the", "slow", "red", "turtle" };
	sort(svec.begin(), svec.end());
	auto end_unique = unique(svec.begin(), svec.end());

	for (const auto & c : svec)
	{
		cout << c<<" " ;
	}
	cout << endl;

	svec.erase(end_unique, svec.end());

	for (const auto & c : svec)
	{
		cout << c<<" ";
	}
	cout << endl;
}

bool isFive(const string &s)
{
	return s.size() >= 5;
}
void partition1013()
{
	vector<string> svec = { "the", "quick", "red", "fox", "jumps", "over", "the", "slow", "red", "turtle" };
	//auto end_partition = partition(svec.begin(), svec.end(), isFive);

	auto end_partition = partition(svec.begin(), svec.end(), [](const string &s) {return s.size() >= 5; });//lambda写法

	//int count = count_if(svec.begin(), svec.end(), [](string s) {return s.size() >= 5; });
	int count = count_if(svec.begin(), svec.end(), bind(isFive, _1));
	
	for (auto iter = svec.begin(); iter != end_partition; ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	cout << count << endl;
}

//小写转大写
void upper_test()
{
	string ss = "abcdefgz";
	transform(ss.begin(), ss.end(), ss.begin(), [](char s) {return s >= 'a' || s <= 'z' ? s - 32 : s; });
	cout << ss << endl;
}

void unique_copy_test1027()
{
	list<string> copy(10);//注意list要分配空间，否则没有地方拷贝，程序core掉
	vector<string> str = { "the", "quick", "red", "fox", "jumps", "over", "the", "slow", "red", "turtle" };
	sort(str.begin(), str.end());
	auto unique_end = unique_copy(str.begin(), str.end(), copy.begin());

	for (auto c : copy)
	{
		cout << c <<" ";
	}
	cout << endl;
	for (auto c : str)
	{
		cout << c << " ";
	}
	cout << endl;

	//for (auto iter = str.begin(); iter != unique_end; ++iter)
	//{
	//	cout << *iter << " ";
	//}
	cout << endl;
}

void text_string_test1029()
{
	ifstream in("test.txt");
	vector<string> txt;

	istream_iterator<string>in_iter(in);
	istream_iterator<string> eof;
	while (in_iter != eof)
	{
		txt.push_back(*in_iter++);
	}
	

	for (auto c : txt)
	{
		cout << c << endl;
	}

	cout << txt[0] << endl;
	
	
}

//输入并进行排序后输出
void iter_copy_sort1030()
{
	istream_iterator<int> in_iter(cin);
	istream_iterator<int> eof;
	ostream_iterator<int> out_iter(cout," ");
	vector<int> vec;

	while (in_iter != eof)
	{
		
		vec.push_back(*in_iter++);
	}
	sort(vec.begin(), vec.end());

	copy(vec.begin(), vec.end(), out_iter);

	cout << endl;

}

void odd_even1033()
{
	ifstream in("test.txt");
	ofstream out_odd("add.txt");
	ofstream out_even("even.txt");

	istream_iterator<int> in_iter(in);
	istream_iterator<int> eof;
	ostream_iterator<int> out_odd_iter(out_odd, " ");
	ostream_iterator<int> out_even_iter(out_even, "\n");


	for_each(in_iter, eof, [&](const int i) {(i % 2 ? out_odd_iter : out_even_iter)++ = i; });

}

void list_algo_test()
{
	list<int> test{ 9,7,5,3,1,1,3,5,7,9 };
	list<int> temp{ 8,6,4,2,2,4,6,8 };

	test.sort();//排序
	temp.sort();
	test.unique();//删除重复
	temp.unique();

	test.merge(temp);//合并并排序---前提：两者都有序

	test.reverse();//翻转
	


	for (auto c : test)
	{
		cout << c << endl;
	}
}

void dici_test113()
{
	map<string, size_t>dici;
	set<string> exclude{ "in","the","and","or" };
	string str;
	while (getline(cin, str))
	{
		if (str == "99")
		{
			break;
		}
		if (exclude.find(str) == exclude.end())
		{
			if (dici.find(str) == dici.end())
			{
				dici.insert(pair<string, size_t>(str, 1));
			}
			else
			{
				++dici[str];
			}
		}
	}


}

void pair_test()
{
	vector<pair<int, string>> test;
	pair<int, string> p;
	istream_iterator<string> in(cin);
	ostream_iterator<string> out(cout, " ");

	bool flag = true;
	int count = 0;
	int pair_int;
	string pair_string;
	while (*in != "999")
	{
		if (flag)
		{
			pair_int = atoi((*in++).c_str());
			flag = false;
			++count;
		}
		else
		{
			pair_string = *in++;
			flag = true;
			++count;
		}
		if (count == 2)
		{
			test.push_back(make_pair(pair_int, pair_string));
			count = 0;
		}
	}

	for (auto c : test)
	{
		cout << c.first << " " << c.second << endl;
	}
}

void word_test1120()
{
	map<string, size_t> dici;
	string str;
	while (getline(cin, str))
	{
		if (str == "999")
			break;
		auto ret = dici.insert({ str,1 });
		if (!ret.second)
		{
			++ret.first->second;
		}
	}

	for (auto c : dici)
	{
		cout << c.first << " " << c.second << endl;
	}

}

class StrBlobPtr;
class StrBlob
{
	friend class StrBlobPtr;
public:
	typedef std::vector<string>::size_type size_type;

	// begin()和end()函数的定义必须在StrBlobPtr类定义之后，否则会报错（StrBlobPtr是不完全类型）
	StrBlobPtr begin();

	StrBlobPtr end();

	//~StrBlob() = delete;
	StrBlob() 
	{
		data = make_shared<std::vector<std::string>>(std::vector<std::string>());
	};
	StrBlob(std::initializer_list<string> list) //initializer_list---列表初始化容器
	{

		data = make_shared<vector<string>>(list);
		
	}

	//StrBlob& operator=(const StrBlob& str)
	//{
	//	this->data = make_shared<std::string>(*str.data);

	//	return *this;
	//}

	void push_back(const std::string& val)
	{
		data->push_back(val);
	}

	void pop_back()
	{
		check(0, "pop_back on empty");
		return data->pop_back();
	}

	std::string& front()
	{
		check(0, "front on empty");
		return data->front();
	}

	std::string& back()
	{
		check(0, "back on empty");
		return data->back();
	}

	size_type size()
	{
		return data->size();
	}

	bool empty()
	{
		return data->empty();
	}


private:

	void check(size_type i, const std::string &msg) const//检测data[i]是否合法，不合法抛出异常
	{
		if (i >= data->size())
		{
			throw out_of_range(msg);
		}
	}

private:
	std::shared_ptr<std::vector<std::string>> data;

};

void StrBlob_122()
{
	StrBlob str{ "123456","abcdef","1a2b3c" };
	
	cout << str.front() << " " << str.back() << endl;
}

class StrBlobPtr
{

public:
	StrBlobPtr() :curr(0) {}

	StrBlobPtr(StrBlob& a, std::size_t size = 0) :wptr(a.data), curr(size) {}

	//返回所指对象的string内容
	std::string deref() const 
	{
		auto p = check(curr, "dereference past end");
		return (*p)[curr];
	}

	//前缀递减,返回递增后的对象的引用
	StrBlobPtr& incr()
	{
		check(curr, "increment past end of StrBlobStr");
		++curr;
		return *this;
	}

	bool operator!=(StrBlobPtr& blob)
	{
		if (this->curr != blob.curr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	//检查成功，则返回一个指向vector的shared_prt
	std::shared_ptr<std::vector<std::string>> check(std::size_t i, const std::string& msg) const 
	{
		auto ret = wptr.lock();
		if (!ret)
		{
			throw std::runtime_error("unbound StrBlobStr");
		}
		if (i >= ret->size())
		{
			throw std::out_of_range(msg);
		}

		return ret;
	}

	std::weak_ptr<std::vector<std::string >> wptr;

	std::size_t curr;//在数组中的当前位置

};

StrBlobPtr StrBlob::begin()
{
	return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end()
{
	auto ret = StrBlobPtr(*this, data->size());
	return ret;
}

void StrBlobPtr_1220()
{
	std::string str;
	StrBlob blob;
	StrBlobPtr pbegin, pend;

	while (getline(std::cin, str))
	{
		if (str == "999")
		{
			break;
		}
		blob.push_back(str);

	}

	for (pbegin = blob.begin(), pend = blob.end(); pbegin != pend; pbegin.incr())
	{
		std::cout << pbegin.deref() << std::endl;
	}

}

void allocator_test1226()
{
	allocator<std::string> alloc;
	auto const pbeg = alloc.allocate(10);
	auto pend = pbeg;
	std::string line;

	while (getline(std::cin, line)&&pend!=pbeg+10)
	{
		alloc.construct(pend++, line);
	}

	while (pend != pbeg)
	{
		std::cout << *--pend << std::endl;
		alloc.destroy(pend);
	}
	alloc.deallocate(pbeg,10);

}


//引用计数
class HasPtr
{
public:
	friend void swap(HasPtr& lhs, HasPtr& rhs);

	HasPtr(const std::string& s = std::string()) :ps(new std::string(s)), i(0),use(new std::size_t (1))//初始化对象，将引用计数置为1
	{
		std::cout << "引用计数置为1" << std::endl;
	}
	HasPtr(const HasPtr& ptr):ps(ptr.ps),i(ptr.i),use(ptr.use)
	{
		++*use;//拷贝构造，引用计数加1
		std::cout << "引用计数加1" << std::endl;
	}


	~HasPtr()
	{
		std::cout << "引用计数减1" << std::endl;
		if (--*use == 0)//析构，引用计数减1
		{//引用计数为0，销毁对象
			std::cout << "销毁对象" << std::endl;
			delete ps;
			delete use;
		}
	}

	HasPtr& operator=(const HasPtr& ptr)
	{
		//auto newptr= new std::string(*ptr.ps);//防止自己赋值自己后先销毁了自己
		//if (this->ps)
		//{
		//	this->~HasPtr();
		//	//delete ps;
		//}

		++*ptr.use;//拷贝赋值运算符，引用计数加1
		std::cout << "右侧引用计数加1" << std::endl;
		--*use;
		if (this->use == 0)
		{
			delete ps;
			delete use;
		}
		std::cout << "左侧引用计数减1" << std::endl;
		this->ps = ptr.ps;
		this->i = ptr.i;
		this->use = ptr.use;
		
		return *this;
	}

	bool operator<(const HasPtr& ptr)
	{
		return *this->ps < *ptr.ps;
	}

	std::string getPs()
	{
		return *this->ps;
	}

private:
	std::string* ps = nullptr;
	int i;

	std::size_t* use;//引用计数

};
void hasptr_test135()
{
	HasPtr test("12345");

	HasPtr copy = test;
	
	std::cout << test.getPs() << endl;
	
}

class Employee
{
public:
	Employee() = default;

	Employee(const Employee&) = delete;

	Employee& operator=(const Employee&) = delete;

	Employee(const std::string& name) :m_name(name) 
	{
		m_ID = ++count;
	}

	unsigned int getID()
	{
		return m_ID;
	}
private:
	static unsigned int count;
	std::string m_name;
	unsigned int m_ID;

};

unsigned int Employee::count = 0;

void employee1318()
{
	Employee test;
	Employee test2("123");
	std::cout << test.getID() << " " << test2.getID() << std::endl;

}


void HasPtr_count1327()
{
	HasPtr test1;
	HasPtr test2 = test1;
	HasPtr test3("123");
	test3 = test2;

	//test1.~HasPtr();
	//test2.~HasPtr();
	//test3.~HasPtr();

}



void treenode1328()
{

	class TreeNode
	{
	public:

		TreeNode() :value(std::string()), count(new int(1)), left(nullptr), right(nullptr) 
		{
			cout << "置1" << endl;
		}

		TreeNode(std::string str) :value(str), count(new int(1)), left(nullptr), right(nullptr)
		{
			cout << "置1" << endl;
		}

		~TreeNode()
		{
			cout << "递减" << endl;
			if (--* count == 0)
			{
				cout << "销毁" << endl;
				delete count;
				delete left;
				delete right;
			}
		}

		TreeNode(const TreeNode& node):value(node.value),left(node.left),right(node.right),count(node.count)
		{
			++* count;
			cout << "递增" << endl;
		}

		TreeNode& operator=(const TreeNode& node)
		{
			cout << "递增" << endl;
			++*node.count;

			this->~TreeNode();

			this->value = node.value;
			this->left = node.left;
			this->right = node.right;
			this->count = node.count;

			return *this;

		}



	private:
		std::string value;
		int* count;
		TreeNode* left;
		TreeNode* right;
	};


	TreeNode test1;
	TreeNode test2();
	TreeNode test3("123");
	TreeNode test4 = test3;
	TreeNode test5("  ");
	test5 = test4;


}

inline void swap(HasPtr& lhs,HasPtr& rhs)
{
	using std::swap;
	swap(lhs.ps, rhs.ps);
	swap(lhs.i,rhs.i);
	swap(lhs.use, rhs.use);
	std::cout << "使用swap" << std::endl;
}

void hasptr_swap1330()
{
	HasPtr test1("123");
	HasPtr test2("abc");

	swap(test1, test2);
	std::cout << test1.getPs() << std::endl;
	std::cout << test2.getPs() << std::endl;

}

void hasptr_sort1331()
{
	vector<HasPtr> vec;
	vec.emplace_back("4");
	vec.emplace_back("2");
	vec.emplace_back("1");
	vec.emplace_back("3");

	std::sort(vec.begin(), vec.end());

	for (auto& i : vec)
	{
		std::cout << i.getPs() << std::endl;
	}

}


void Massage_Folder()
{
	

}

int main(int argc, char** argv)
{
	//vector_half();
	//scores();
	//array_init_vector();
	//num_count();
	//try_catch();
	//static_test();
	//if_string_func();
	//ad() = 5;
	//printf("%d,%d", *pt(), ad());
	//printf("%d", (*get())[2]);//(*get())[2]：解引用指向数组的指针，再使用下标来获取对象的值
	//func_pt();
	//salse_data_book();
	//library();
	//bug_test();
	//Account::rate(5.12);
	//printf("%f", Account::rate());
	/*第八章*/
	//ifstream_8_1(argc,argv);
	//stringstream_test();
	//容器
	//list_to_vector();
	//swap_test();
	//insert_test();
	//emplace_test();
	//test926();
	//forward_list_erase_after927();
	//字符串操作
	//char_to_string941();
	//string_test946();
	//string_find_test947();
	//stack_test952();
	//泛型算法
	//algo_test101();
	//elimDups109();
	//partition1013();
	//upper_test();
	//unique_copy_test1027();
	//text_string_test1029();
	//iter_copy_sort1030();
	//odd_even1033();
	//list_algo_test();
	//关联容器
	//dici_test113();
	//pair_test();
	//word_test1120();
	//智能指针
	//StrBlob_122();
	//StrBlobPtr_1220();
	//allocator_test1226();
	//拷贝控制
	//hasptr_test135();
	//employee1318();
	//HasPtr_count1327();
	//treenode1328();
	//hasptr_swap1330();
	//hasptr_sort1331();
	Massage_Folder();//拷贝控制示例

	system("pause");
	return 0;
}
