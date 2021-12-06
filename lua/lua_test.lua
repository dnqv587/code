--单行注释
--[[
多行注释
--]]

print("hello world!")
print(3.21)

local temp="hello world"
print(temp)

local list={
	[1]="hello",
	key="world"
}

--创建索引
print(list)
print(list[1])
print(list.key)--字符串索引才可以用点
print(list["key"])


--函数
function test_func()
	print("test_".."func")--字符串加法
	print(list[1])
	print(list.key)
end

test_func()

local i=1
for i=1,10,1 do
	print(i)
end

local i=1
while i<=10 do
	print(i)
	i=i+1
end
