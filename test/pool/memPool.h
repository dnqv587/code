#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <stdint.h>

//小块内存数据---细分为多个数据块，由整个数据块释放
struct poolData
{
	poolData() :last(nullptr),end(nullptr),next(nullptr),failed(0){
	}
	unsigned char* last;//保存当前数据块中内存分配指针的当前位置
	unsigned char* end;//保存内存块的结束位置
	poolData* next;//内存池由多块内存块组成，指向下一个数据块的位置
	uint32_t failed;//当前数据块内存不足引起分配失败的次数
};

//大块内存数据---整个释放
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
	
	std::size_t m_max;//当前数据块最大可分配的内存大小（Byte）
	poolData* m_current;//当前正在使用的数据块的指针
	poolData* m_data;//内存池当前的数据区指针的结构体
	poolLargeData* m_large;//pool中指向大数据块的指针（大数据块是指size>max的数据块），使用链表实现
};
