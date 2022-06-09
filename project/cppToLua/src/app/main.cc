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

	//加载脚本
	/* 将字符串加载到lua虚拟机当中
	*参数：虚拟机，lua代码字符串，长度，命名
	* 返回：0--成功，非0--错误
	*/
	int ret=luaL_loadbuffer(L, lua_buf, sizeof(lua_buf), "helloworld");//相似.luaL_loadstring
	if (ret != 0)
	{
		//取
		const char* msg = lua_tolstring(L, -1,NULL);
		printf("load script failed %s\n", msg);
	}

	
	lua_close(L);
	return 0;
}

