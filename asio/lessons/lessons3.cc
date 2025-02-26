#include <iostream>
#include "asio/io_context.hpp"
#include <thread>
#include <chrono>

using namespace asio;

io_context context;

void sleepfunc()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
    std::cout<<"3"<<std::endl;
}

void func()
{
    std::cout<<"begin"<<std::endl;
    context.post(sleepfunc);
    std::cout<<"end"<<std::endl;
}
int main()
{


    context.wrap(func)();

    context.run();


    return 0;
}