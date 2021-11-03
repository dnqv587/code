#include <stdio.h>

int main(int argc, char **argv)
{
	union 
	{
		short s;
		char bytes[sizeof(short)];
	}endian;
	endian.s = 0x0102;
	if (endian.bytes[0] == 0x01 && endian.bytes[1] == 0x02)
		printf("big endian\n");
	if (endian.bytes[0] == 0x02 && endian.bytes[1] == 0x01)
		printf("little endian\n");
}