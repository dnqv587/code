
#include <iostream>
#include <zinx.h>
using namespace std;

//2-д���ܴ�����
class Echo :public AZinxHandler
{
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override  //override�����ֱ�ʾ��ǰ������д�˻�����麯��
	{
		//���Ե���׼���
		GET_REF2DATA(BytesMsg, input, _oInput);//����һ��BytesMsg������Ϊinput��ȡ_oInput�е��ֽ���

		cout << input.szData << endl;

		return nullptr;
	}

	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return nullptr;
	}

};

class Test::

//����׼���룬���Ա�׼���
int main()
{
	//1-��ʼ�����
	ZinxKernel::ZinxKernelInit();
	ZinxKernel::ZinxKernelFini();
	return 0;
}