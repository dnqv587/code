#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

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


/*后缀表达式运算
* 
*	对于数字：进栈
* 
*	对于符号：
*			1.从栈中弹出有操作数
*			2.从栈中弹出左操作数
*			3.根据符号进行运算
*			4.将运算结果压入栈中
* 
*	遍历结束：栈中的唯一数字为计算结果
* 
*/

int main(int argc, char* argv[])
{

	infillToSuffix();


	return 0;
}