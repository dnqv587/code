#include <iostream>
#include "zookeeperHelper.h"
#include <future>
#include <thread>

int main()
{
    ZookeeperClient zk("127.0.0.1:2181");
    zk.connect(10,[](const ZK_SessionEvent event){
        if(ZK_SessionEvent::Connecting == event)
        {
            std::cout<<"正在连接！"<<std::endl;
        }
        else if(ZK_SessionEvent::Connected == event)
        {
            std::cout<<"连接成功！"<<std::endl;
        }
        else if(ZK_SessionEvent::Expired == event )
        {
            std::cout<<"会话过期"<<std::endl;
        }
    });
    zk.waitConnected();
    //zk.createNode("/test","child","{data:2}",ZK_NodeMode::Persistent,ZK_ACL::Open);
    std::future<bool> a = std::async(std::launch::async,[&]()
    {
        return zk.raceMaster("/zkRace","test1");
    });

    std::future<bool> b = std::async(std::launch::async,[&]()
    {
        return zk.raceMaster("/zkRace","test2");
    });

    std::cout<<"test1"<<std::boolalpha<<a.get()<<",test2"<<b.get()<<std::endl;


    std::this_thread::sleep_for(std::chrono::seconds(100));
    zk.close();

	return 0;
}
