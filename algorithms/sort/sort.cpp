#include <iostream>
#include <string>
using namespace std;


void myPut(int s[],int len )
{
	for (int i = 0; i < len; i++)
	{
		cout << s[i] << "  ";
	}
	cout << endl;
}

//√∞≈›≈≈–Ú
void  bubble(int s[],int len)
{
	
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len - i; j++)
		{
			if (s[j + 1] > s[j])
			{
				int temp = s[j];
				s[j] = s[j + 1];
				s[j + 1] = temp;
			}
		}
	}
}

//—°‘Ò≈≈–Ú
void select(int s[],int len)
{
	for (int i = 0; i < len; i++)
	{
		int min = i;
		for (int j = i + 1; j < len; j++)
		{
			if (s[min] > s[j])
			{
				min = j;
			}
		}
		int temp = s[i];
		s[i] = s[min];
		s[min]=temp;
	}
}

//≤Â»Î≈≈–Ú
void insert(int s[], int len)
{
	for (int i = 1; i < len; i++)
	{
		if (s[i - 1] > s[i])
		{
			int temp = s[i];
			int j = i-1;
			for (; j >= 0 && s[j - 1] > temp; j--)
			{
				s[j + 1] = s[j];
			}
			s[j] = temp;
		}
	}
}


int main()
{
	int s[] = {5,64,19,654,216,848,3,5494,16,45,265,7896,164,134,111,123,32 }, len = sizeof(s)/sizeof(s[0]);
	//bubble(s,len);
	//select(s, len);
	insert(s, len);
	myPut(s,len);
	return 0;
}

