#include <iostream>
#include "clientOP.h"

using namespace std;

int run()
{ 
	int nSel = -1;
	printf("\n  /*************************************************************/");
	printf("\n  /*************************************************************/");
	printf("\n  /*     1.密钥协商                                            */");
	printf("\n  /*     2.密钥校验                                            */");
	printf("\n  /*     3.密钥注销                                            */");
	printf("\n  /*     4.密钥查看                                            */");
	printf("\n  /*     0.退出系统                                            */");
	printf("\n  /*************************************************************/");
	printf("\n  /*************************************************************/");
	printf("\n\n  选择:");

	scanf("%d", &nSel);
	while (getchar() != '\n');

	return nSel;
}

int main(int argc, char* argv[])
{
	clientOP op;
	while (1)
	{
		int cmd = run();
		switch (cmd)
		{
		case 1:
			//秘钥协商
			op.secKeyAgree();
			break;
		case 2:
			//秘钥校验
			break;
		case 3:
			//秘钥注销
			break;
		case 4:
			//秘钥查看
			break;
		case 0:
			//退出系统
			break;
		}

	}

	cout << "客户端退出。。。" << endl;
	return 0;

}