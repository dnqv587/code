--����ע��
--[[
����ע��
--]]

print("hello world!")
print(3.21)

local temp="hello world"
print(temp)

local list={
	[1]="hello",
	key="world"
}

--��������
print(list)
print(list[1])
print(list.key)--�ַ��������ſ����õ�
print(list["key"])


--����
function test_func()
	print("test_".."func")--�ַ����ӷ�
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
