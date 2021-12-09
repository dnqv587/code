
#include <iostream>
#include <zinx.h>
using namespace std;

//2-写功能处理类
class Echo :public AZinxHandler
{
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override  //override保留字表示当前函数重写了基类的虚函数
	{
		//回显到标准输出
		GET_REF2DATA(BytesMsg, input, _oInput);//定义一个BytesMsg的引用为input，取_oInput中的字节流

		cout << input.szData << endl;

		return nullptr;
	}

	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return nullptr;
	}

};

class Test::

//读标准输入，回显标准输出
int main()
{
	//1-初始化框架
	ZinxKernel::ZinxKernelInit();
	ZinxKernel::ZinxKernelFini();
	return 0;
}