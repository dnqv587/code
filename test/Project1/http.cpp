#include "HttpRequest.h"
#include <iostream>
#include <Windows.h>

int main(int argc, char* argv[])
{
	HttpRequest httpReq("172.18.44.236", 9505);

	std::string res = httpReq.HttpGet("/api/info_changed");
	std::cout << res << std::endl;
	//Sleep(1000);
	
	res = httpReq.HttpPost("/postsomething/", HttpRequest::genJsonString("something", 100));
	std::cout << res << std::endl;
	return 0;
}