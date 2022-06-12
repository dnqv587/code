local str1 = "hello world!\n"
-- 字符替换
local str2 = string.gsub(str1, "hello", "nihao")
print(str2)

io.write(str2)
-- 运行时转换
print("10" + 2)
print("10" .. 2) --  ..字符串相加
print(tonumber("10") + 2) -- tonumber  显式的将字符转换为数值
print("10" .. tostring(2)) -- tostring
-- 读取一行
local line = io.read()
print(type(line))
-- # 获取字符串长度
print(#"12345")

-- 表
print("---表---")
local table1 = {} -- 创建一个table
local k = "x"
table1[k] = 10
table1[20] = k
print(table1["x"] .. table1[20])

local table2 = table1 -- table1和table2引用了同一个table
print(table2["x"] .. table2[20])
table1 = nil -- table1不在引用table
table2 = nil -- table2不在引用table
-- table被回收
A = {}
A["x"] = 10086
print(A["x"])
print(A.x) -- 相当于A["x"]
function Func(n)
    io.write("函数调用:" .. n * n .. '\n')
end
A.func = Func
A.func(10)

-- 读取10行内容并存储到表中
Str = {}
for i = 1, 10 do
    Str[i] = io.read()
end
for i = 1, #Str do -- #Str 获取Str表的长度
    print(Str[i] .. ";")
end

print(Str[#Str]) -- 打印列表Str最后一个值
Str[#Str] = nil -- 删除表尾元素
Str[#Str + 1] = 0 -- 将0插入到表尾
