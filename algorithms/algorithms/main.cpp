#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

#include "../dataStructure/sqStack.hpp"
#include "../dataStructure/array.hpp"

using namespace std;


/*中缀转后缀
* 
* 对于数字：直接输出
* 
* 对于符号：
*			左括号：进栈
*			运算符号：与栈顶符号进行优先级比较：若栈顶符号优先级低，此符号进栈（默认左括号优先级最低）；若栈顶符号优先级不低，则将栈顶符号弹出并输出，之后进栈
*			右括号：将栈顶符号弹出并输出，直到匹配左括号，将左括号和右括号同时舍弃
*			遍历结束：将栈中所有符号弹出并输出
*/
DArray<char> infillToSuffix()
{
	string infill = "(1+2*3+(3-1))-(8-2*3)/2";
	DsqStack<char> oper(100);
	DArray<char> suffix(100);

	for (const auto& c : infill)
	{
		if (c >= '0' && c <= '9')
		{
			suffix.pushBack(c);
			//suffix.pushBack(' ');
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
				//suffix.pushBack(' ');
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
				//suffix.pushBack(' ');
				oper.pop();
				oper.push(c);
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
				//suffix.pushBack(' ');
				oper.pop();
				oper.push(c);
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
				//suffix.pushBack(' ');
				oper.pop();
				oper.push(c);
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
				//suffix.pushBack(' ');
				oper.pop();
				oper.push(c);
			}
			break;
		}
	}
	while (!oper.isEmpty())
	{
		suffix.pushBack(oper.top());
		//suffix.pushBack(' ');
		oper.pop();
	}

	
	for (int i = 0; i < suffix.getSize(); ++i)
	{
		cout << suffix[i] << " ";
	}
	cout << endl;

	return suffix;
}


/*后缀表达式运算
* 
*	对于数字：进栈
* 
*	对于符号：
*			1.从栈中弹出右操作数
*			2.从栈中弹出左操作数
*			3.根据符号进行运算
*			4.将运算结果压入栈中
* 
*	遍历结束：栈中的唯一数字为计算结果
* 
*/

void operation(DArray<char> suffix)
{
	DsqStack<string> oper(100);
	//DArray<char> suffix(100);
	int right;
	int left;

	for (int i = 0; i < suffix.getSize(); ++i)
	{
		if (suffix[i] >= '0'&&suffix[i] <= '9')
		{
			oper.push(string (1,suffix[i]));
		}
		switch (suffix[i])
		{
		case '+':
			right = atoi(oper.top().c_str());
			oper.pop();
			left = atoi(oper.top().c_str());
			oper.pop();
			oper.push(to_string(left + right));
			break;
		case '-':
			right = atoi(oper.top().c_str());
			oper.pop();
			left = atoi(oper.top().c_str());
			oper.pop();
			oper.push(to_string(left - right));
			break;
		case '*':
			right = atoi(oper.top().c_str());
			oper.pop();
			left = atoi(oper.top().c_str());
			oper.pop();
			oper.push(to_string(left * right));
			break;
		case '/':
			right = atoi(oper.top().c_str());
			oper.pop();
			left = atoi(oper.top().c_str());
			oper.pop();
			oper.push(to_string(left / right));
			break;
		}
	}

	cout << oper.top() << endl;
}

int main(int argc, char* argv[])
{

	operation(infillToSuffix());


	return 0;
}