#include <exception>

class socket_error :public std::exception
{
public:
	socket_error() noexcept
		:_what("socket_error")
	{
	}

	const char* what() const noexcept override
	{
		return _what;
	}
protected:
	socket_error(const char* const what)
		:_what(what)
	{

	}
private:
	const char* const _what;
};

class create_socket_fail :public socket_error
{
public:
	create_socket_fail() noexcept
		:socket_error("create_socket_fail")
	{

	}
};

class close_socket_fail :public socket_error
{
public:
	close_socket_fail() noexcept
		:socket_error("close_socket_fail")
	{

	}
};