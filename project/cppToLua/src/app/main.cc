#include <lua.hpp>

const char* lua_buf = R"(
functon hellworld()
	printf('hello world')
end
helloworld()
)";

int main(int argc, char* argv[])
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//���ؽű�
	/* ���ַ������ص�lua���������
	*�������������lua�����ַ��������ȣ�����
	* ���أ�0--�ɹ�����0--����
	*/
	int ret=luaL_loadbuffer(L, lua_buf, sizeof(lua_buf), "helloworld");//����.luaL_loadstring
	if (ret != 0)
	{
		//ȡ
		const char* msg = lua_tolstring(L, -1,NULL);
		printf("load script failed %s\n", msg);
	}

	
	lua_close(L);
	return 0;
}

