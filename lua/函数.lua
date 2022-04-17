
--[[
    规则：
    函数无需定义返回类型，并且返回任意类型和任意数量的数值
    函数的参数无需定义类型
    可以定义变量，把函数直接赋值给它，从而获得相同的功能
]]
function funName(num1,num2)
    print("函数1:"..num1,num2);
    return num1+num2;
end

result=funName(100,200);
print("函数1返回："..result);

--函数赋值给变量
fun2=funName;
ret=fun2(66,88);
print("函数1赋值给变量返回："..ret);

--局部函数    不能被其他的lua文件访问
local function fun2(num1,num2)
    print("函数2:"..num1,num2);
    return num1+num2;
end

--函数的多返回
function GetNumber()
    return 10,20,30;
end
res1,res2,res3=GetNumber();
print(res1,res2,res3);

--函数作为参数
function printInfo(result)
    print("计算的结果："..result)
end

function AddNum(num1,num2,printFunc)
    local result=num1+num2;
    printFunc(result);
end

AddNum(10,20,printInfo);