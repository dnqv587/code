#include <stdio.h>
#include<event2/event.h>
int main()
{
	int i = 0;
	//��ȡ��ǰ֧�ֵķ���
	const char** p = event_get_supported_methods();
	printf("supported methods:");
	while (p[i] != NULL)
	{
		printf("%s\t", p[i++]);
	}
	printf("\n");
	//��ȡ�ػ��ڵ�
	struct event_base* base = event_base_new();
	if (base == NULL)
	{
		printf("new event_base error\n");
		return -1;
	}
	const char* pp = event_base_get_method(base);
	printf("current method:%s\n", pp);
	event_base_free(base);
	return 0;
}