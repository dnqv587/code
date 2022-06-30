#include <lua.hpp>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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


void  stringTest()
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
			return ;
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

}


static int add2(lua_State* L)
{
	printf("add2\n");
	double op1 = luaL_checknumber(L, 1);
	double op2 = luaL_checknumber(L, 2);
	lua_pushnumber(L, op1 + op2);
	return 1;
}

static int luayield(lua_State* L)
{
	printf("luayield\n");
	return lua_yield(L, 0);
}

static int sub2(lua_State* L)
{
	printf("sub2\n");
	double op1 = luaL_checknumber(L, 1);
	double op2 = luaL_checknumber(L, 2);
	lua_pushnumber(L, op1 - op2);
	return 1;
}

static int luaresume(lua_State* L)
{
	int res;
	return lua_resume(L, NULL, 0, &res);
}

const char* testfunc = "print(\"luabg\") luayield() print(add2(1.0,2.0)) print(sub2(20.1,19)) luayield() print(\"end\")";

const char buf[] = R"( 
function hellworld() 
	print("1")
	luayield()
	print("2")  
end 
function my_pow(x,y)
	print("3")
	luayield()
	return x^y
end
hellworld() 
res=my_pow(1,2)

print(res)
)";

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_State* newL = lua_newthread(L);

	lua_register(newL, "luayield", luayield);
	lua_register(newL, "luaresume", luaresume);
	lua_register(newL, "add2", add2);
	lua_register(newL, "sub2", sub2);
	if (luaL_loadstring(newL, buf))
	{
		printf("Failed to invoke.\n");
	}
	sleep(1);
	//printf("resume bf\n");
	luaresume(newL);
	luaresume(newL);
	luaresume(newL);
	//sleep(1);
	//printf("resume end\n");
	//luaresume(newL);
	//sleep(1);
	//luaresume(newL);
	lua_close(L);
	return ;
}

int main(int argc, char* argv[])
{
	test();

	getchar();
	return 0;
}

