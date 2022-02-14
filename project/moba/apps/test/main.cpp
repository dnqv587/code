#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include "../../src/netbus/netbus.h"

using namespace std;

int main(int argc, char* argv[])
{
	netbus* test = netbus::instance();

	test->start_tcp_server(8080);
	test->start_tcp_server(6080);
	test->run();

	return 0;
}