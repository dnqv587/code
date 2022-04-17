

--if语句
--单分支语句
if(true) then       --逻辑表达式括号可以不写
    --[body]
end

--双分支语句
if(true) then
    --[body]
else
    --[body]
end

--多分支语句
if(true) then
    --[body]
elseif(true) then
    --[body]
else
    --[body]
end

--while循环
i=0;
while (i<10) do
    print(i);
    i=i+1;
end
--repeat关键词
--先循环实现，后判断；相当于do..。while
i=0;
repeat 
    print(i);
    i=i+1;
until (i>10)   --结束条件

--for循环
for i=1,10 do
    print(i);
end
--使用for循环的步长
for i=1,10,2 do
    print(i);
end

--泛型for循环（使用迭代器关键字）
--ipairs
array={2,4,6,8,10};
for index,value in ipairs(array) do
    print(index,value);   --k为数组的下表，v为数组的内容
end

array2={num1="张三",num2="李四",num3="王五"};  --具备键值对的表
for index, value in pairs(array2) do  --定义循环输出具备“键值对”集合的数据，必须使用pairs关键字
    print(index,value);
end
--[[
    ipairs和pairs的区别：
    ipairs遍历顺序的集合（中间序号不能断，遇到nil数值就停止），一般用于遍历数组类型的集合
    pairs遍历集合中所有的数据（序号可以中断），一般用于键值对类型集合的数据
]]

--break关键字
for i=1, 100 do 
    print(i);
    if i==10 then
        break;
    end
end