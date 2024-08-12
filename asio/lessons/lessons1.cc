#include <iostream>
#include <system_error>
#include "asio/ip/address.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/io_context.hpp"

void server()
{
    asio::io_context io;
    asio::ip::tcp::acceptor acc(io, asio::ip::tcp::endpoint{asio::ip::address_v6::any(),8088});
}
void client()
{
    std::error_code ec;
    asio::ip::address addr = asio::ip::address::from_string("127.0.0.1",ec);
    if(ec)
    {
        std::cout << ec.message() << std::endl;
    }
    asio::ip::tcp::endpoint ep(addr, 8088);

    asio::io_context io;
    asio::ip::tcp::socket sock(io);



}
int main(int argc, char const *argv[])
{

}