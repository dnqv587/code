local str1 = "hello world!\n"
-- �ַ��滻
local str2 = string.gsub(str1, "hello", "nihao")
print(str2)

io.write(str2)
-- ����ʱת��
print("10" + 2)
print("10" .. 2) --  ..�ַ������
print(tonumber("10") + 2) -- tonumber  ��ʽ�Ľ��ַ�ת��Ϊ��ֵ
print("10" .. tostring(2)) -- tostring
-- ��ȡһ��
local line = io.read()
print(type(line))
-- # ��ȡ�ַ�������
print(#"12345")

-- ��
print("---��---")
local table1 = {} -- ����һ��table
local k = "x"
table1[k] = 10
table1[20] = k
print(table1["x"] .. table1[20])

local table2 = table1 -- table1��table2������ͬһ��table
print(table2["x"] .. table2[20])
table1 = nil -- table1��������table
table2 = nil -- table2��������table
-- table������
A = {}
A["x"] = 10086
print(A["x"])
print(A.x) -- �൱��A["x"]
function Func(n)
    io.write("��������:" .. n * n .. '\n')
end
A.func = Func
A.func(10)

-- ��ȡ10�����ݲ��洢������
Str = {}
for i = 1, 10 do
    Str[i] = io.read()
end
for i = 1, #Str do -- #Str ��ȡStr��ĳ���
    print(Str[i] .. ";")
end

print(Str[#Str]) -- ��ӡ�б�Str���һ��ֵ
Str[#Str] = nil -- ɾ����βԪ��
Str[#Str + 1] = 0 -- ��0���뵽��β
