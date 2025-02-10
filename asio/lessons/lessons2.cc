#include <iostream>
#include "asio/ip/tcp.hpp"
#include "asio/io_service.hpp"
#include <memory>
#include <thread>
#include <future>
#include <system_error>
#include <functional>
//***********************************************
// name      : 异步echo示例
// brief     : 
// attention : None
//***********************************************
using namespace asio;

void service()
{
	io_service service;
	ip::tcp::endpoint ep(ip::tcp::v4(),2001);
	ip::tcp::acceptor acc(service,ep);

	std::shared_ptr<ip::tcp::socket> sock = std::make_shared<ip::tcp::socket>(service);

	acc.async_accept(*sock,[sock,&acc,&service](std::error_code ec){
	  if(ec)
		  return;
	  while(true)
	  {
		  char data[64];
		  memset(data,0x00,sizeof(data));
		  size_t len = sock->read_some(buffer(data));
		  if (len > 0)
		  {
			  sock->write_some(buffer(data));
		  }
	  }
	});
	service.run();

}

void client()
{
	io_service service;
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"),2001);
	ip::tcp::socket  sock(service);
	char buf [1024];
	sock.async_connect(ep,[&buf,&sock](std::error_code code){
		if(code)
			return;
	  while(true)
	  {
		  ::memset(buf, 0x00, sizeof(buf));
		  ::read(STDIN_FILENO, buf, sizeof(buf));
		  sock.write_some(buffer(buf));
		  ::memset(buf, 0x00, sizeof(buf));
		  sock.read_some(buffer(buf));
		  std::cout << "read=" << buf << std::endl;
	  }
	});

	service.run();
}

std::string HostNameToIP(std::string_view host)
{
	io_service s;
	ip::tcp::resolver resolver(s);
	ip::tcp::resolver::query q(host.data(), "80");
	ip::tcp::resolver::iterator iter = resolver.resolve( q);
	ip::tcp::endpoint ep = *iter;
	std::cout << ep.address().to_string() << ":" << ep.port()
			  << "/" << ep.protocol().type() << std::endl;

	return ep.address().to_string();
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