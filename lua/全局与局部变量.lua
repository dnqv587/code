--[[
1.lua变量默认是全局性的，定义局部变量需要用关键字local来修饰。
2.lua语句块（if、for等）与函数中，定义的默认都是也就是全局变量。
3.lua中也可以直接使用do ... end 来直接定义一个语句块
4.如果需要删除一个全局变量，只需要给变量赋值nil
]]

if (true) then
    local num1=30;  --局部变量
    num2=20;        --全局变量
    print(num1);
end
print(num2);
num2=nil;  --释放资源
