#include <iostream>
#include "asio/ip/tcp.hpp"
#include "asio/io_service.hpp"
#include <memory>
#include <thread>
#include <future>
#include <system_error>

using namespace asio;

void service()
{
	io_context context;

}

void client()
{

}

int main(int argc, char const *argv[])
{


	if(argc > 1 && ::strcmp(argv[1], "c") == 0)
	{
		std::cout<<"client"<<std::endl;
		auto c = std::async(client);
		c.wait();
	}
	else
	{
		std::cout<<"server"<<std::endl;
		auto s = std::async(service);
		s.wait();
	}
}