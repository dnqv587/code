#include "my_data.h"
#include "./1/Sales_item.h"
#include "./7/Sales_data.h"


using namespace std;
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
	algo_test101();
	system("pause");
	return 0;
}
