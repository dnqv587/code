



#include "netbus.h"

using namespace std;

//��������
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
˵��������������ݵ��ڴ�
������
	1���������¼��ľ��
	2�������������ڴ棬����������
	3��׼���õ��ڴ棬ͨ��uv_buf_t������event_loop������ڴ��ַ
*/
	static void tcp_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		if (handle->data != NULL)//�ͷ��ϴζ���������
		{
			free(handle->data);
			handle->data = nullptr;
		}
		handle->data = malloc(suggested_size + 1);//suggested_size����Ĵ�С��+1�š�\0��
		buf->base = (char*)handle->data;
		buf->len = suggested_size;
	}



	static void close_cb(uv_handle_t* handle)
	{
		if (handle->data)
		{
			free(handle->data);//�ͷ����һ�ζ���������
			handle->data = nullptr;
		}

		free(handle);

		std::cout << "close client" << std::endl;
	}

	static void shutdown_cb(uv_shutdown_t* req, int status)
	{

		uv_close((uv_handle_t*)req->handle, close_cb);//�ر��׽���
		free(req);
	}

	//д�¼��ص�
	static void tcp_write_cb(uv_write_t* req, int status)
	{
		if (status == 0)
			std::cout << "send success" << std::endl;

		free(req);
	}

	/*
˵�������¼��ص�
������
	1�����¼����
	2�������˶����ֽڵ�����
	3���������ڵ�buf
*/
	static void tcp_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
	{
		//���ӶϿ�
		if (nread < 0)
		{
			uv_shutdown_t* req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
			memset(req, 0x00, sizeof(uv_shutdown_t));
			uv_shutdown(req, stream, shutdown_cb);//�Ͽ�����
			return;
		}

		buf->base[nread] = 0;

		std::cout << "recv" << nread << ":" << buf->base << std::endl;

		//д����
		uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));
		uv_buf_t send_buf = uv_buf_init((char*)"send data", sizeof("send data"));
		uv_write(req, stream, &send_buf, 1, tcp_write_cb);
	}

	static void connection_cb(uv_stream_t* server, int status)
	{
		std::cout << "new client coming" << std::endl;

		//����ͻ���
		uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		memset(client, 0x00, sizeof(uv_tcp_t));
		uv_tcp_init(uv_default_loop(), client);
		uv_accept(server, (uv_stream_t*)client);

		//�洢�ͻ�����Ϣ
		

		//����event_loop,��������ĳ���¼�

		/*
		˵����������¼�
		������
			1������
			2������������ݵ��ڴ�
		*/
		uv_read_start((uv_stream_t*)client, tcp_alloc_cb, tcp_read_cb);
	}

}

void netbus::start_tcp_server(int port)
{
	m_tcp = new uv_tcp_t;
	memset(m_tcp, 0x00, sizeof(uv_tcp_t));

	// Tcp ��������;
	uv_tcp_init(uv_default_loop(), m_tcp);
	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", port, &addr); // ip��ַ, �˿�
	int ret = uv_tcp_bind(m_tcp, (const struct sockaddr*)&addr, 0);
	if (ret != 0) {
		printf("bind error \n");
		delete m_tcp;
		return;
	}

	// ��event loop�����������������ǵ�listen������������ӵ�ʱ��;
	// event loop�ͻ�����û�ָ�������������connection_cb;
	/*������
		1:����
		2����������Ŀ��SOMAXCONN�������Ŀ
	 */
	uv_listen((uv_stream_t*)m_tcp, SOMAXCONN, connection_cb);
	//listen->data = (void*)TCP_SOCKET;
}

void netbus::run()
{
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
