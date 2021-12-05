#ifndef __ZINX_STUDY_H__
#define ____ZINX_STUDY_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <zinx.h>
using namespace std;

class Echo :public AZinxHandler
{
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput)override
	{
		//回显到标准输出
		GET_REF2DATA(BytesMsg, input, _oInput);

		cout << input.szData << endl;
		return nullptr;
	}

	virtual IZinxMsg* InternelHandle(IZinxMsg& _oNextMsg)override
	{
		//回显到标准输出
		return nullptr;
	}
};



#endif