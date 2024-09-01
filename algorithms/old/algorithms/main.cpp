#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

#include "../dataStructure/sqStack.hpp"
#include "../dataStructure/array.hpp"
#include "../dataStructure/sqQueue.hpp"

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
DArray<string> infillToSuffix()
{
	string infill = "100+100*(999+1)-50/5";
	DsqStack<string> oper(100);
	DsqQueue<char> digit(10);
	DArray<string> suffix(100);
	string dig;

	for (const auto& c : infill)
	{
	
		if (c >= '0' && c <= '9')
		{
			//suffix.pushBack(c);
			digit.push(c);
			continue;
		}
		if (c == '(')
		{
			oper.push(string(1, c));
			continue;
		}
		if (c == ')')
		{
			string temp = oper.top();
			oper.pop();
			while (temp != "(")
			{
				suffix.pushBack(temp);
				//suffix.pushBack(' ');
				temp = oper.top();
				oper.pop();
			}
			continue;
		}
		string top = "";
		if (!oper.isEmpty())
		{
			top = oper.top();
		}

		
		switch (c)
		{
		case '+':
			while (!digit.isEmpty())
			{
				dig.push_back(digit.pop());
			}
			suffix.pushBack(dig);
			if (top == "(" || oper.isEmpty())
			{
				oper.push(string(1, c));
			}
			else
			{
				suffix.pushBack(oper.top());
				//suffix.pushBack(' ');
				oper.pop();
				oper.push(string(1,c));
			}
			break;
		case '-':
			while (!digit.isEmpty())
			{
				dig.push_back(digit.pop());
			}
			suffix.pushBack(dig);
			if (top == "(" || oper.isEmpty())
			{
				oper.push(string(1, c));
			}
			else
			{
				suffix.pushBack(oper.top());
				//suffix.pushBack(' ');
				oper.pop();
				oper.push(string(1, c));
			}
			break;
		case '*':
			while (!digit.isEmpty())
			{
				dig.push_back(digit.pop());
			}
			suffix.pushBack(dig);
			if (top == "(" || top == "-" || top == "+" || oper.isEmpty())
			{
				oper.push(string(1, c));
			}
			else
			{
				suffix.pushBack(oper.top());
				//suffix.pushBack(' ');
				oper.pop();
				oper.push(string(1, c));
			}
			break;
		case '/':
			while (!digit.isEmpty())
			{
				dig.push_back(digit.pop());
			}
			suffix.pushBack(dig);
			if (top == "(" || top == "-" || top == "+" || oper.isEmpty())
			{
				oper.push(string(1, c));
			}
			else
			{
				suffix.pushBack(oper.top());
				//suffix.pushBack(' ');
				oper.pop();
				oper.push(string(1, c));
			}
			break;
		}
		dig.clear();
	}

	while (!digit.isEmpty())
	{
		dig.push_back(digit.pop());
	}
	suffix.pushBack(dig);
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

void operation(DArray<string> suffix)
{
	DsqStack<string> oper(100);
	//DArray<char> suffix(100);
	int right;
	int left;


	for (int i = 0; i < suffix.getSize(); ++i)
	{

		if (suffix[i] == "+")
		{
			right = atoi(oper.top().c_str());
			oper.pop();
			left = atoi(oper.top().c_str());
			oper.pop();
			oper.push(to_string(left + right));
		}
		else if (suffix[i] == "-")
		{
			right = atoi(oper.top().c_str());
			oper.pop();
			left = atoi(oper.top().c_str());
			oper.pop();
			oper.push(to_string(left - right));
		}
		else if (suffix[i] == "*")
		{
			right = atoi(oper.top().c_str());
			oper.pop();
			left = atoi(oper.top().c_str());
			oper.pop();
			oper.push(to_string(left * right));
		}
		else if (suffix[i] == "/")
		{
			right = atoi(oper.top().c_str());
			oper.pop();
			left = atoi(oper.top().c_str());
			oper.pop();
			oper.push(to_string(left / right));
		}
		else
			oper.push(suffix[i]);

	}

	cout << oper.top() << endl;
}

int main(int argc, char* argv[])
{

	operation(infillToSuffix());


	return 0;
}