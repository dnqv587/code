#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>

#include <uv.h>

#include "session.h"
#include "session_uv.h"

using namespace std;


void session_uv::close()
{

}

void session_uv::send_data(unsigned char* body, int len)
{

}

const char* session_uv::get_address(int* client_port)
{
	return NULL;
}

session* session_uv::create()
{
	session_uv* uv_s = new session_uv();
	uv_s->init();
	return (session*)uv_s;
}

void session_uv::destroy(session* s)
{
	session_uv* uv_s = (session_uv*)s;
	uv_s->exit();

	delete uv_s;
}

void session_uv::init()
{
	memset(c_address, 0x00, sizeof(c_address));
	c_port = 0;
	recved = 0;
}

void session_uv::exit()
{

}


