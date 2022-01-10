#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include "../dataStructure/sqStack.hpp"
#include "../dataStructure/array.hpp"

using namespace std;


/*��׺ת��׺
* 
* �������֣�ֱ�����
* 
* ���ڷ��ţ�
*			�����ţ���ջ
*			������ţ���ջ�����Ž������ȼ��Ƚϣ���ջ���������ȼ��ͣ��˷��Ž�ջ��Ĭ�����������ȼ���ͣ�����ջ���������ȼ����ͣ���ջ�����ŵ����������֮���ջ
*			�����ţ���ջ�����ŵ����������ֱ��ƥ�������ţ��������ź�������ͬʱ����
*			������������ջ�����з��ŵ��������
*/
void infillToSuffix()
{
	string infill = "8+(3-1)*5";
	DsqStack<char> oper(100);
	DArray<char> suffix(100);

	for (const auto& c : infill)
	{
		if (c >= '0' && c <= '9')
		{
			suffix.pushBack(c);
			continue;
		}
		if (c == '(')
		{
			oper.push(c);
			continue;
		}
		if (c == ')')
		{
			char temp = oper.top();
			oper.pop();
			while (temp != '(')
			{
				suffix.pushBack(temp);
				temp = oper.top();
				oper.pop();
			}
			continue;
		}
		char top = ' ';
		if (!oper.isEmpty())
		{
			top = oper.top();
		}
		switch (c)
		{
		case '+':
			if (top == '('||oper.isEmpty())
			{
				oper.push(c);
			}
			else
			{
				suffix.pushBack(oper.top());
				oper.pop();
			}
			break;
		case '-':
			if (top == '(' || oper.isEmpty())
			{
				oper.push(c);
			}
			else
			{
				suffix.pushBack(oper.top());
				oper.pop();
			}
			break;
		case '*':
			if (top == '(' || top == '-' || top == '+' || oper.isEmpty())
			{
				oper.push(c);
			}
			else
			{
				suffix.pushBack(oper.top());
				oper.pop();
			}
			break;
		case '/':
			if (top == '(' || top == '-' || top == '+' || oper.isEmpty())
			{
				oper.push(c);
			}
			else
			{
				suffix.pushBack(oper.top());
				oper.pop();
			}
			break;
		}
	}
	while (!oper.isEmpty())
	{
		suffix.pushBack(oper.top());
		oper.pop();
	}


	for (int i = 0; i < suffix.getSize(); ++i)
	{
		cout << suffix[i];
	}
	cout << endl;
}


/*��׺���ʽ����
* 
*	�������֣���ջ
* 
*	���ڷ��ţ�
*			1.��ջ�е����в�����
*			2.��ջ�е����������
*			3.���ݷ��Ž�������
*			4.��������ѹ��ջ��
* 
*	����������ջ�е�Ψһ����Ϊ������
* 
*/

int main(int argc, char* argv[])
{

	infillToSuffix();


	return 0;
}