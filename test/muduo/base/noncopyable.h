#pragma once
/*
�����������޷�ʹ�ÿ�������Ϳ���������
*/

class noncopyalbe
{
protected:
	noncopyalbe() = default;
	~noncopyalbe() = default;

private:
	noncopyalbe(const noncopyalbe&) = delete;
	noncopyalbe& operator=(const noncopyalbe&) = delete;

};
