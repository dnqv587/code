#pragma once
#include <vector>


class Observer;

//���۲���:data
class Oberservalbe
{
public:
	//֪ͨ������֪ͨ���й۲���
	void notify();

	//ע�ắ��
	void _register(Observer* observer);

private:
	//�۲����б�---�������еĹ۲���
	std::vector<Observer*> m_observers;
};

//�۲���
class Observer
{
public:
	//���麯����չʾ����
	virtual void update() = 0;

};

