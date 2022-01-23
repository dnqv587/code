#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>


#include "libuv.h"


static uv_loop_t* event_loop = NULL;//事件循环对象
static uv_udp_t server;//UDP的句柄
static uv_timer_t timer;//timer句柄


//-----------UDP--------
//分配回调函数
static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	if (handle->data != NULL)
	{
		free(handle->data);
		handle->data = NULL;
	}
	handle->data = malloc(suggested_size + 1);//suggested_size建议的大小；+1放‘\0’
	buf->base = (char*)handle->data;
	buf->len = suggested_size;
}


//send处理回调函数
static void udp_send_cb(uv_udp_send_t* req, int status)
{
	if (status == 0)//发送成功
	{
		std::cout << "send success" << std::endl;
	}
	free(req);

}

//recv处理回调函数
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
	//写数据
	uv_udp_send_t* req = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
	uv_udp_send(req, handle, &w_buf, 1, addr, udp_send_cb);

}



void  UDP()
{
	event_loop = uv_default_loop();//创建默认的事件loop
	memset(&server, 0x00, sizeof(uv_udp_t));

	uv_udp_init(event_loop, &server);//初始化udp

	//绑定端口
	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", 8080, &addr);
	uv_udp_bind(&server, (sockaddr*)&addr, 0);

	//告诉时间循环，要管理的事件
	uv_udp_recv_start(&server, alloc_cb, udp_recv_cb);

	

	uv_run(event_loop, UV_RUN_DEFAULT);//开启事件循环等待事件的发生

}

//-----------------Timer-----------
static void timerTest_cb(uv_timer_t* handle)
{
	std::cout << "timer called" << std::endl;
	//uv_timer_stop(handle); //停止timer句柄
}

void timer_test()
{
	event_loop = uv_default_loop();//创建默认的事件loop

	uv_timer_init(event_loop, &timer);//初始化timer句柄

	uv_timer_start(&timer, timerTest_cb, 5000, 0);//timeout:第一次间隔运行时间；repeat：之后间隔的运行时间--0为只运行一次

	uv_run(event_loop, UV_RUN_DEFAULT);

}


//自定义timer

struct Timer
{
	uv_timer_t uv_timer;//libuv处理句柄
	void (*timer_cb)(void* udata);
	void* udata;
	int repeat_count;//-1一直循环
};

//构建Timer结构体
Timer* alloc_timer(void (*timer_cb)(void* udata), void* udata, int repeat_count)
{
	Timer* t = (Timer*)malloc(sizeof(Timer));
	memset(t, 0x00, sizeof(Timer));

	t->timer_cb = timer_cb;
	t->udata = udata;
	t->repeat_count = repeat_count;

	uv_timer_init(uv_default_loop(), &t->uv_timer); //初始化句柄

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
		t->timer_cb(t->udata);//递归
	}
	if (t->repeat_count == 0)
	{
		uv_timer_stop(&t->uv_timer); //停止timer句柄
		free_timer(t);//释放
	}

	t->timer_cb(t->udata);//repeat_count为-1，一直执行
}

/*
参数：
void (*timer_cb)(void* udata)：回调函数，当timer出发时回调
void* udata：用户传的自定义的数据结构，回调函数执行的时候的udata就是这个udata
float after_sec：多少秒开始执行
int repeat_count：执行多少次，-1代表一直执行
返回:timer的句柄
*/
//构建timer
Timer* schedule(void (*timer_cb)(void* udata), void* udata,int after_sec,int repeat_count)
{
	Timer* t = alloc_timer(timer_cb, udata, repeat_count);

	//libuv启动timer
	t->uv_timer.data = t;//存储Timer数据
	uv_timer_start(&t->uv_timer, (uv_timer_cb)timer_cb, after_sec , after_sec );

	return t;
}


//回调函数只执行一次
Timer* schedule_once(void (*timer_cb)(void* udata), void* udata, int after_sec)
{
	return schedule(timer_cb, udata, after_sec, 1);
}

//取消timer
void  cancel_timer(Timer* t)
{
	if (t->repeat_count == 0)//全部触发完成
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
	event_loop = uv_default_loop();//创建默认的事件loop

	Timer* t = schedule(callback_timer, (char*)"Hello", 1000, 5);


	uv_run(event_loop, UV_RUN_DEFAULT);
}


