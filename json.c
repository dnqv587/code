#include <stdio.h>
#include <json-c/json.h>

int main()
{
	const char* str = { \"name\":\"jack\",\"age\":22,\"sex\":\"male\"};
	struct json_object* obj = json_tokener_parse(str);

	printf("%s", json_boject_to_json_string(obj));
	return 0;
}