#include <iostream>
#include <string>
using namespace std;

template <typename T>
//���ݽ���
void Swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

template <typename T>
//ѡ������
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

//��������
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

//�����ַ���
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


//ģ�������---�Զ�����������---------------
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

template<> bool compare(Person& a, Person& b)//���廯ģ��
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

//��ģ��---------------

template<class NAMETYPE,class IDTYPE = int> //��ģ�������Ĭ��ֵ
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

//��ģ������������----------------

void doWork1(person<string, int>& p)//����ָ������
{
	p.showPerson();
}

template<class T1,class T2>
void doWork2(person<T1, T2>& p)//����ģ�廯
{
	p.showPerson();
}

template<class T>
void doWork3(T& p)//������ģ�廯
{
	p.showPerson();
}

void testClass()
{
	//��ģ�岻��ʹ���Զ������Ƶ�
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


//��ģ����̳�-----------

template<class T>
class Base
{
public:
	T m_A;
};

class Son1 :Base<int>//����Ҫָ��������ģ���Ա���������ͣ����ܸ���������ڴ�
{
public:
	
};

template<class T1,class T2>
class Son2 :Base< T1>//ʹ��ģ����ָ������ģ���Ա
{
public:
	T2 m_b;
};



//��ģ���Ա������ʵ��-------------

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
person1<T1,T2>::person1(T1 name, T2 id)   //����1
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
	person1<string, int >p1("ģ��", 1);
	p1.showPerson();
}

//��ģ������Ԫ����

template<class T1, class T2 >
class person2;
template<class T1, class T2 >
void showPerson2(person2<T1, T2>& p);//��Ԫ��������ʵ�ֱ�������������---��ʽ1
//------
template<class T1, class T2 >
class person2;
template<class T1, class T2 >
void showPerson3(person2<T1, T2>& p)//��ʽ2,��Ԫ������������ʵ��дһ��
{
	cout << "name:" << p.m_name << endl;
	cout << "id:" << p.m_id << endl;
}

template<class T1, class T2 >
class person2
{
public:
	//��Ԫ��������ʵ��
	friend void showPerson(person2<T1,T2>&p)
	{
		cout << "name:" << p.m_name << endl;
		cout << "id:" << p.m_id << endl;
	}

	//��Ԫ��������ʵ��
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
	person2<string, int >p1("��ģ������Ԫ����", 1);
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
