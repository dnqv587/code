#include <iostream>
#include <zookeeper.h>
#include <condition_variable>
#include <mutex>
#include <string.h>
#include <thread>

struct context
{
	std::condition_variable cond;
	std::mutex mutex;
};

int main()
{
	context ctx;

	std::unique_lock<std::mutex> lock(ctx.mutex);
	zhandle_t* zk = zookeeper_init("localhost:2181", [](zhandle_t*, int type, int state, const char* path, void* v)
	{
	  auto* ctx = (context*)v;
	  {
		  std::lock_guard<std::mutex> lock(ctx->mutex);
		  ctx->cond.notify_all();
	  }

	}, 10000, nullptr, &ctx, 0);

	ctx.cond.wait(lock);


	int retval = 0;
	char buffer[128];
	int buffer_length = sizeof(buffer);
	memset(buffer, 0, sizeof(buffer));
	struct ACL ACL_ANYONE[] = {{ZOO_PERM_ALL, ZOO_ANYONE_ID_UNSAFE}};
	struct ACL_vector ACL_ANYONE_VEC = {1, ACL_ANYONE};

	auto ret = zoo_create(zk,"/test","value",5,&ACL_ANYONE_VEC,ZOO_EPHEMERAL,buffer,buffer_length);

//	using namespace std::chrono_literals;
//	std::this_thread::sleep_for(2s);

	if(ret ==  ZOK)
	{
		std::cout<<"ret OK!"<<std::endl;
	}

	memset(buffer, 0, sizeof(buffer));
	retval = zoo_get(zk, "/test", 0, buffer, &buffer_length, nullptr);

	std::cout<<"data:"<<buffer<<std::endl;

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(100s);
	zookeeper_close(zk);


	return 0;
}