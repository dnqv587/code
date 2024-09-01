#include <iostream>
#include <string>
using namespace std;

template <typename T>
//数据交换
void Swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

template <typename T>
//选择排序
void sort(T &list, int len)
{
	for (int i = 0; i < len; i++)
	{
		int min=i;
		for (int j = i + 1; j < len; j++)
		{
			if (list[min] > list[j])
				min = j;
		}
		if(min!=i)
		Swap(list[i], list[min]);
	}
}

//测试整型
void testInit()
{
	int s[] = { 4,6,8,9,15,35,64,1655,34,54,646554,3215,354 }, len = sizeof(s) / sizeof(s[0]);
	sort(s, len);
	for (int i = 0; i < len; i++)
	{
		cout << s[i] << ',';
	}
	cout << endl;
}

//测试字符型
void testChar()
{
	char s[] = "saoifnioagsafdasdas";
	int len = sizeof(s) / sizeof(s[0]);
	sort(s, len);
	for (int i = 0; i < len; i++)
	{
		cout << s[i] << ',';
	}
	cout << endl;
}


//模板局限性---自定义数据类型---------------
class Person
{
public:
	Person(string name,int id) 
	{
		m_name = name;
		m_id = id;
	}
	string  m_name;
	int m_id;
};


template<class T>
bool compare(T& a, T& b)
{
	if (a == b)
	{
		return true;
	}
	return false;
}

template<> bool compare(Person& a, Person& b)//具体化模板
{
	if (a.m_id == b.m_id)
	{
		return true;
	}
	return false;
}

void testLimit()
{
	int s1 = 1;
	int s2 = 1;

	Person p1("a", 1);
	Person p2("b", 2);

	cout << compare<int >(s1, s2) << endl;

	cout << compare<Person>(p1, p2) << endl;
}

//类模板---------------

template<class NAMETYPE,class IDTYPE = int> //类模板可以有默认值
class person
{
public:
	person(NAMETYPE name, IDTYPE id)
	{
		m_name = name;
		m_id = id;
	}
	void showPerson()
	{
		cout << "name:" << m_name << endl;
		cout << "id:" << m_id << endl;
	}

	NAMETYPE  m_name;
	IDTYPE m_id;
};

class c1
{
public:
	void show1()
	{
		cout << "c1" << endl;
	}
};

class c2
{
public:
	void show2()
	{
		cout << "c2" << endl;
	}
};

template <class T>
class myClass
{
public:
	void func1()
	{
		m_obj.show1();
	}
	void func2()
	{
		m_obj.show2();
	}

	T m_obj;
};

//类模板作函数参数----------------

void doWork1(person<string, int>& p)//传入指定类型
{
	p.showPerson();
}

template<class T1,class T2>
void doWork2(person<T1, T2>& p)//参数模板化
{
	p.showPerson();
}

template<class T>
void doWork3(T& p)//整个类模板化
{
	p.showPerson();
}

void testClass()
{
	//类模板不可使用自动类型推导
	person<string, int> p1("abc", 1);
	p1.showPerson();

	myClass <c1>obj1;
	obj1.func1();
	myClass <c2>obj2;
	obj2.func2();

	doWork1(p1);
	doWork2(p1);
	doWork3(p1);
}


//类模板与继承-----------

template<class T>
class Base
{
public:
	T m_A;
};

class Son1 :Base<int>//必须要指定基类中模板成员的数据类型，才能给子类分配内存
{
public:
	
};

template<class T1,class T2>
class Son2 :Base< T1>//使用模板来指定基类模板成员
{
public:
	T2 m_b;
};



//类模板成员的类外实现-------------

template<class T1, class T2 >
class person1
{
public:
	person1(T1 name, T2 id);

	void showPerson();

	T1  m_name;
	T2 m_id;
};

template<class T1,class T2>
person1<T1,T2>::person1(T1 name, T2 id)   //方法1
{
	m_name = name;
	m_id = id;
}

template<class T1, class T2>
void person1<T1, T2>::showPerson()
{
	cout << "name:" << m_name << endl;
	cout << "id:" << m_id << endl;
}

void testtemp()
{
	person1<string, int >p1("模板", 1);
	p1.showPerson();
}

//类模板与友元函数

template<class T1, class T2 >
class person2;
template<class T1, class T2 >
void showPerson2(person2<T1, T2>& p);//友元函数类外实现必须有以上声明---方式1
//------
template<class T1, class T2 >
class person2;
template<class T1, class T2 >
void showPerson3(person2<T1, T2>& p)//方式2,友元函数的声明和实现写一起
{
	cout << "name:" << p.m_name << endl;
	cout << "id:" << p.m_id << endl;
}

template<class T1, class T2 >
class person2
{
public:
	//友元函数类内实现
	friend void showPerson(person2<T1,T2>&p)
	{
		cout << "name:" << p.m_name << endl;
		cout << "id:" << p.m_id << endl;
	}

	//友元函数类外实现
	friend void showPerson2<>(person2<T1, T2>& p);

	friend void showPerson3<>(person2<T1, T2>& p);

public:
	person2(T1 name, T2 id)
	{
		m_name = name;
		m_id = id;
	}

	T1  m_name;
	T2 m_id;
};

template<class T1, class T2 >
void showPerson2(person2<T1, T2>& p)
{
	cout << "name:" << p.m_name << endl;
	cout << "id:" << p.m_id << endl;
}


void test273()
{
	person2<string, int >p1("类模板与友元函数", 1);
	showPerson(p1);
	showPerson2(p1);
	showPerson3(p1);
}

int main()
{
	//testChar();
	//testLimit();
	//testClass();
	//testtemp();
	test273();

	system("pause");
	return 0;
}
