#pragma once

template <typename T>
class CircularBuffer
{
public:
	CircularBuffer(int num)
		:_num(num)
	{
		_head = (T*>)malloc(sizeof(T) * _num);
		_front = _head;
	}
	~CircularBuffer()
	{
		free(_front);
	}

	void push(T& val)
	{
		if (_head + _num == _front)
		{
			*_front = val;
			++_front;
		}
		else
		{
			*_front = val;
			_front = _head;
		}
	}


private:
	const T* _head;
	T* _front;
	int _num;
};