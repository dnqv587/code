#include <iostream>
#include "zookeeperHelper.h"


int main()
{
    ZookeeperClient zk("127.0.0.1:2181");
    zk.connect(10,[](ZK_SessionEvent event,std::string_view node){
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
    std::cout<<"get data:"<<zk.getData("/test/child")<<std::endl;
    zk.close();

	return 0;
}