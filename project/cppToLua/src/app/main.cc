#include <lua.hpp>
#include <string.h>

#define LUA_LIB

const char lua_buf[] = R"( 
function hellworld() 
	print("hello world") 
end 
function my_pow(x,y)
	return x^y
end
--hellworld() 

)";


int main(int argc, char* argv[])
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	{
		//加载脚本

		/* 将字符串加载到lua虚拟机当中
		*参数：虚拟机，lua代码字符串，长度，命名
		* 返回：0--成功，非0--错误
		*/
		int ret = luaL_loadbuffer(L, lua_buf, strlen(lua_buf), "helloworld");//相似.luaL_loadstring
		if (ret != 0)
		{
			//出错
			if (lua_isstring(L, -1))
			{
				//取栈顶元素
				const char* msg = lua_tolstring(L, -1, NULL);
				printf("load script failed %s\n", msg);
				lua_pop(L, 1);

			}
			return -1;
		}
		//状态机，参数数量，返回值数量，错误返回
		if (lua_pcall(L, 0, 0, 0))//执行脚本
		{
			//出错
			const char* msg = lua_tolstring(L, -1, NULL);
			printf("call function failed %s\n", msg);
			lua_pop(L, 1);
		}
		ret=lua_getglobal(L, "my_pow");
		printf("type:%d\n", ret);
		if (!lua_isfunction(L, -1))
		{
			printf("not fuction");
		}
		lua_pushnumber(L, 2);
		lua_pushnumber(L, 8);
		if (lua_pcall(L, 2, 1, 0))//执行脚本
		{
			//出错
			const char* msg = lua_tolstring(L, -1, NULL);
			printf("call function failed %s\n", msg);
			lua_pop(L, 1);
		}
		if (lua_isnumber(L, -1))
		{
			lua_Number val = lua_tonumber(L, -1);
			printf("my_pow ret=%f", val);
		}
		
	}


	
	lua_close(L);
	return 0;
}

