#pragma once

template <typename T>
class CircularBuffer
{
public:
	CircularBuffer(int num)
		:_num(num)
	{
		_head = (std::shared_ptr<T*>)malloc(sizeof(std::shared_ptr<T*>) * _num);
		_front = _head;
	}
	~CircularBuffer()
	{
		free(_front);
	}

	void push(std::shared_ptr<T*>&& val)
	{
		if (_head + _num == _front)
		{
			*_front = std::move(val);
			++_front;
		}
		else
		{
			*_front = std::move(val);
			_front = _head;
		}
	}


private:
	std::shared_ptr<T*> _head;
	std::shared_ptr<T*> _front;
	int _num;
};