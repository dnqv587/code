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

template <class T>
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


int main()
{
	testChar();
	return 0;
}
