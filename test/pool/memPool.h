#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <stdint.h>

//С���ڴ�����---ϸ��Ϊ������ݿ飬���������ݿ��ͷ�
struct poolData
{
	poolData() :last(nullptr),end(nullptr),next(nullptr),failed(0){
	}
	unsigned char* last;//���浱ǰ���ݿ����ڴ����ָ��ĵ�ǰλ��
	unsigned char* end;//�����ڴ��Ľ���λ��
	poolData* next;//�ڴ���ɶ���ڴ����ɣ�ָ����һ�����ݿ��λ��
	uint32_t failed;//��ǰ���ݿ��ڴ治���������ʧ�ܵĴ���
};

//����ڴ�����---�����ͷ�
struct poolLargeData
{
	poolLargeData() :next(nullptr), alloc(nullptr) {

	}
	poolLargeData* next;
	void* alloc;
};

class memPool
{
public:
	memPool(std::size_t size);

	~memPool();

	void reset();

private:
	
	std::size_t m_max;//��ǰ���ݿ����ɷ�����ڴ��С��Byte��
	poolData* m_current;//��ǰ����ʹ�õ����ݿ��ָ��
	poolData* m_data;//�ڴ�ص�ǰ��������ָ��Ľṹ��
	poolLargeData* m_large;//pool��ָ������ݿ��ָ�루�����ݿ���ָsize>max�����ݿ飩��ʹ������ʵ��
};
