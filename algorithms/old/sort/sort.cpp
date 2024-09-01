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


/*
ѡ������
ʱ�临�Ӷȣ�n^2
�ռ临�Ӷȣ�1
*/
void select(int* s, int len)
{
	for (int i = 0; i < len-1; ++i)
	{
		int min = i;
		for (int j = i+1; j < len; ++j)
		{
			if (s[min] > s[j])
			{
				min = j;
			}
		}
		int temp;
		temp = s[min];
		s[min] = s[i];
		s[i] = temp;
	}
}


/*
ð������
ʱ�临�Ӷȣ�n^2
�ռ临�Ӷȣ�1
*/
void bubble(int* s, int len)
{
	for (int i = 0; i < len; ++i)
	{
		for (int j = 0; j < len-i-1; ++j)
		{
			if (s[j] > s[j + 1])
			{
				int temp;
				temp = s[j];
				s[j] = s[j + 1];
				s[j + 1] = temp;
			}
		}
	}
}

/*
��������
ʱ�临�Ӷȣ�n^2
�ռ临�Ӷȣ�1
*/
void insert(int* s, int len)
{
	for (int i = 1; i < len; ++i)
	{
		for (int j = i; j >0; --j)
		{
			if (s[j] < s[j - 1])
			{
				int temp;
				temp = s[j];
				s[j] = s[j - 1];
				s[j - 1] = temp;
			}
		}
	}
}

/*
ϣ������
ʱ�临�Ӷȣ�n^1.3
�ռ临�Ӷȣ�1
*/
void shell(int* s, int len)
{
	for (int gap = len/2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < len; i++)
		{
			for (int j = i; j > gap - 1; j -= gap)
			{
				if (s[j] < s[j - gap])
				{
					int temp = s[j];
					s[j] = s[j - gap];
					s[j - gap] = temp;
				}
			}
		}
	}


}

/*
�鲢����
ʱ�临�Ӷȣ�nlogn
�ռ临�Ӷȣ�n
*/
void _merge(int* s, int begin, int mid, int end)
{
	int* temp = new int[end - begin + 1];
	int pos = 0;
	int left = begin;
	int right = mid + 1;
	while (left <= mid && right <= end)
	{
		if (s[left] > s[right])
		{
			temp[pos] = s[right];
			++pos;
			++right;
		}
		else
		{
			temp[pos] = s[left];
			++pos;
			++left;
		}
	}
	while (left <= mid)
	{
		temp[pos] = s[left];
		++pos;
		++left;
	}
	while (right <= end)
	{
		temp[pos] = s[right];
		++pos;
		++right;
	}
	for (int i = 0; i < pos; ++i)
	{
		s[begin + i] = temp[i];
	}

	delete[] temp;
}
void merge(int* s, int begin,int end)
{
	if (begin>end)
	{
		return;
	}
	int mid = begin + (end - begin) / 2;
	merge(s, begin, mid);//���
	merge(s, mid + 1, end);//�ұ�
	_merge(s, begin, mid, end);//�ϲ�

}

int main1()
{
	int s[] = {2,4,6,8,10,1,3,5,7,9,0 }, len = sizeof(s)/sizeof(s[0]);
	//bubble(s,len);
	//select(s, len);
	//insert(s, len);
	//shell(s, len);
	merge(s,0, len);
	myPut(s,len);
	system("pause");
	return 0;
}

#define MAX_MEM_SIZE 100
int GetMem(int iLen, void** ppMem)
{
	if (NULL == ppMem)
	{
		return -1;
	}
	if (iLen <= 0) {
		return 0;
	}
	else if (iLen < MAX_MEM_SIZE)
	{
		*ppMem = malloc(iLen);
		return iLen;
	}
	else {
		*ppMem = malloc(MAX_MEM_SIZE);
		return MAX_MEM_SIZE;
	}
}

const unsigned char ucMask = 0x7F;
bool test(unsigned char ucUserType)
{
	unsigned char Mask = ucUserType & (~ucMask);
	if ((ucUserType & (~ucMask)) == (~ucMask))
	{
		return true;
	}
	else
		return false;
}

int main()
{
	cout << test(0xAF) << endl;
}