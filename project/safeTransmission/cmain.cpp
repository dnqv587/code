#include <iostream>
#include "clientOP.h"

using namespace std;

int run()
{
	int nSel = -1;
	printf("\n  /*************************************************************/");
	printf("\n  /*************************************************************/");
	printf("\n  /*     1.��ԿЭ��                                            */");
	printf("\n  /*     2.��ԿУ��                                            */");
	printf("\n  /*     3.��Կע��                                            */");
	printf("\n  /*     4.��Կ�鿴                                            */");
	printf("\n  /*     0.�˳�ϵͳ                                            */");
	printf("\n  /*************************************************************/");
	printf("\n  /*************************************************************/");
	printf("\n\n  ѡ��:");

	scanf("%d", &nSel);
	while (getchar() != '\n');

	return nSel;
}

int main(int argc, char* argv[])
{
	while (1)
	{
		int cmd = run();
		switch (cmd)
		{
		case 1:
			//��ԿЭ��
			break;
		case 2:
			//��ԿУ��
			break;
		case 3:
			//��Կע��
			break;
		case 4:
			//��Կ�鿴
			break;
		case 0:
			//�˳�ϵͳ
			break;
		}

	}

	cout << "�ͻ����˳�������" << endl;
	return 0;

}