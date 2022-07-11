#pragma once

template <typename T>
class CircularBuffer
{
public:
	CircularBuffer(int num)
		:_num(num)
	{
		_front = new T[_num];
	}
	~CircularBuffer()
	{
		delete[]_front;
	}

	void push(const T& val)
	{
		if (_num)
		{
			*(_front + _num) = val;
			--_num;
		}
		else
		{
			*_front = val;
			++_front;
		}
	}


private:
	T* _front;
	int _num;
};