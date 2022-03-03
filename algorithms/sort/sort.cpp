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
选择排序
时间复杂度：n^2
空间复杂度：1
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
冒泡排序
时间复杂度：n^2
空间复杂度：1
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
插入排序
时间复杂度：n^2
空间复杂度：1
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
希尔排序
时间复杂度：n^1.3
空间复杂度：1
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
归并排序
时间复杂度：nlogn
空间复杂度：n
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
	merge(s, begin, mid);//左边
	merge(s, mid + 1, end);//右边
	_merge(s, begin, mid, end);//合并

}

int main()
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

