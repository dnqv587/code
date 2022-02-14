



#include "netbus.h"

using namespace std;

//单例对象
static netbus* g_instance = nullptr;
netbus* netbus::instance()
{
	if (nullptr == g_instance)
	{
		g_instance = new netbus;
		return g_instance;
	}
	return nullptr;
}


extern "C"
{

	/*
说明：申请读入数据的内存
参数：
	1：发生读事件的句柄
	2：建议分配多大的内存，来保存数据
	3：准备好的内存，通过uv_buf_t，告诉event_loop申请的内存地址
*/
	static void tcp_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		if (handle->data != NULL)//释放上次读到的数据
		{
			free(handle->data);
			handle->data = nullptr;
		}
		handle->data = malloc(suggested_size + 1);//suggested_size建议的大小；+1放‘\0’
		buf->base = (char*)handle->data;
		buf->len = suggested_size;
	}



	static void close_cb(uv_handle_t* handle)
	{
		if (handle->data)
		{
			free(handle->data);//释放最后一次读到的数据
			handle->data = nullptr;
		}

		free(handle);

		std::cout << "close client" << std::endl;
	}

	static void shutdown_cb(uv_shutdown_t* req, int status)
	{

		uv_close((uv_handle_t*)req->handle, close_cb);//关闭套接字
		free(req);
	}

	//写事件回调
	static void tcp_write_cb(uv_write_t* req, int status)
	{
		if (status == 0)
			std::cout << "send success" << std::endl;

		free(req);
	}

	/*
说明：读事件回调
参数：
	1：读事件句柄
	2：读到了多少字节的数据
	3：数据所在的buf
*/
	static void tcp_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
	{
		//连接断开
		if (nread < 0)
		{
			uv_shutdown_t* req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
			memset(req, 0x00, sizeof(uv_shutdown_t));
			uv_shutdown(req, stream, shutdown_cb);//断开连接
			return;
		}

		buf->base[nread] = 0;

		std::cout << "recv" << nread << ":" << buf->base << std::endl;

		//写数据
		uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));
		uv_buf_t send_buf = uv_buf_init((char*)"send data", sizeof("send data"));
		uv_write(req, stream, &send_buf, 1, tcp_write_cb);
	}

	static void connection_cb(uv_stream_t* server, int status)
	{
		std::cout << "new client coming" << std::endl;

		//接入客户端
		uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		memset(client, 0x00, sizeof(uv_tcp_t));
		uv_tcp_init(uv_default_loop(), client);
		uv_accept(server, (uv_stream_t*)client);

		//存储客户端信息
		

		//告诉event_loop,让他管理某个事件

		/*
		说明：管理读事件
		参数：
			1：对象
			2：申请读入数据的内存
		*/
		uv_read_start((uv_stream_t*)client, tcp_alloc_cb, tcp_read_cb);
	}

}

void netbus::start_tcp_server(int port)
{
	m_tcp = new uv_tcp_t;
	memset(m_tcp, 0x00, sizeof(uv_tcp_t));

	// Tcp 监听服务;
	uv_tcp_init(uv_default_loop(), m_tcp);
	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", port, &addr); // ip地址, 端口
	int ret = uv_tcp_bind(m_tcp, (const struct sockaddr*)&addr, 0);
	if (ret != 0) {
		printf("bind error \n");
		delete m_tcp;
		return;
	}

	// 让event loop来做监听管理，当我们的listen句柄上有人连接的时候;
	// event loop就会调用用户指定的这个处理函数connection_cb;
	/*参数：
		1:对象
		2：监听的数目，SOMAXCONN：最大数目
	 */
	uv_listen((uv_stream_t*)m_tcp, SOMAXCONN, connection_cb);
	//listen->data = (void*)TCP_SOCKET;
}

void netbus::run()
{
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
