#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>


#include "libuv.h"


static uv_loop_t* event_loop = NULL;//�¼�ѭ������
static uv_udp_t server;//UDP�ľ��
static uv_timer_t timer;//timer���


//-----------UDP--------
//����ص�����
static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	if (handle->data != NULL)
	{
		free(handle->data);
		handle->data = NULL;
	}
	handle->data = malloc(suggested_size + 1);//suggested_size����Ĵ�С��+1�š�\0��
	buf->base = (char*)handle->data;
	buf->len = suggested_size;
}


//send����ص�����
static void udp_send_cb(uv_udp_send_t* req, int status)
{
	if (status == 0)//���ͳɹ�
	{
		std::cout << "send success" << std::endl;
	}
	free(req);

}

//recv����ص�����
static void udp_recv_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags)
{
	char ip[128];
	uv_ip4_name((sockaddr_in*)addr, ip, sizeof(ip));
	int port = ntohs(((sockaddr_in*)addr)->sin_port);
	printf("ip:%s:%d nread = % d\n", ip, port, nread);
	char* str_buf = (char*)handle->data;
	str_buf[nread] = '\0';
	std::cout << "recv:" << str_buf << std::endl;


	uv_buf_t w_buf;

	w_buf = uv_buf_init((char*)"send data", sizeof("send data"));
	//д����
	uv_udp_send_t* req = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
	uv_udp_send(req, handle, &w_buf, 1, addr, udp_send_cb);

}



void  UDP()
{
	event_loop = uv_default_loop();//����Ĭ�ϵ��¼�loop
	memset(&server, 0x00, sizeof(uv_udp_t));

	uv_udp_init(event_loop, &server);//��ʼ��udp

	//�󶨶˿�
	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", 8080, &addr);
	uv_udp_bind(&server, (sockaddr*)&addr, 0);

	//����ʱ��ѭ����Ҫ������¼�
	uv_udp_recv_start(&server, alloc_cb, udp_recv_cb);

	

	uv_run(event_loop, UV_RUN_DEFAULT);//�����¼�ѭ���ȴ��¼��ķ���

}

//-----------------Timer-----------
static void timerTest_cb(uv_timer_t* handle)
{
	std::cout << "timer called" << std::endl;
	//uv_timer_stop(handle); //ֹͣtimer���
}

void timer_test()
{
	event_loop = uv_default_loop();//����Ĭ�ϵ��¼�loop

	uv_timer_init(event_loop, &timer);//��ʼ��timer���

	uv_timer_start(&timer, timerTest_cb, 5000, 0);//timeout:��һ�μ������ʱ�䣻repeat��֮����������ʱ��--0Ϊֻ����һ��

	uv_run(event_loop, UV_RUN_DEFAULT);

}


//�Զ���timer

struct Timer
{
	uv_timer_t uv_timer;//libuv������
	void (*timer_cb)(void* udata);
	void* udata;
	int repeat_count;//-1һֱѭ��
};

//����Timer�ṹ��
Timer* alloc_timer(void (*timer_cb)(void* udata), void* udata, int repeat_count)
{
	Timer* t = (Timer*)malloc(sizeof(Timer));
	memset(t, 0x00, sizeof(Timer));

	t->timer_cb = timer_cb;
	t->udata = udata;
	t->repeat_count = repeat_count;

	uv_timer_init(uv_default_loop(), &t->uv_timer); //��ʼ�����

	return t;
}

void free_timer(Timer* t)
{
	free(t);
}

void timer_cb(uv_timer_t* handle)
{
	Timer* t = (Timer*)handle->data;
	if (t->repeat_count > 0)
	{
		--t->repeat_count;
		t->timer_cb(t->udata);//�ݹ�
	}
	if (t->repeat_count == 0)
	{
		uv_timer_stop(&t->uv_timer); //ֹͣtimer���
		free_timer(t);//�ͷ�
	}

	t->timer_cb(t->udata);//repeat_countΪ-1��һֱִ��
}

/*
������
void (*timer_cb)(void* udata)���ص���������timer����ʱ�ص�
void* udata���û������Զ�������ݽṹ���ص�����ִ�е�ʱ���udata�������udata
float after_sec�������뿪ʼִ��
int repeat_count��ִ�ж��ٴΣ�-1����һֱִ��
����:timer�ľ��
*/
//����timer
Timer* schedule(void (*timer_cb)(void* udata), void* udata,int after_sec,int repeat_count)
{
	Timer* t = alloc_timer(timer_cb, udata, repeat_count);

	//libuv����timer
	t->uv_timer.data = t;//�洢Timer����
	uv_timer_start(&t->uv_timer, (uv_timer_cb)timer_cb, after_sec , after_sec );

	return t;
}


//�ص�����ִֻ��һ��
Timer* schedule_once(void (*timer_cb)(void* udata), void* udata, int after_sec)
{
	return schedule(timer_cb, udata, after_sec, 1);
}

//ȡ��timer
void  cancel_timer(Timer* t)
{
	if (t->repeat_count == 0)//ȫ���������
	{
		return;
	}
	uv_timer_stop(&t->uv_timer);
	free(t);
}

void callback_timer(void* data)
{
	char* str = (char*)data;
	std::cout << str << std::endl;
}

void timer_indi()
{
	event_loop = uv_default_loop();//����Ĭ�ϵ��¼�loop

	Timer* t = schedule(callback_timer, (char*)"Hello", 1000, 5);


	uv_run(event_loop, UV_RUN_DEFAULT);
}


