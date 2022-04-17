--[[
算符运算符：+  -  *   /  %  ^
关系运算符：<   <=   >   >=   ==  ~=
注意：nil只与自身相等
      对于table、函数、userdata，lua是做引用比较的，即：只有当他们引用同一个对象时才认为他们相等
逻辑运算符：and  or  not
注意：
    所有的逻辑运算符将false、nil视为假，其他都视为真
断路规则：
    对于操作符and来说，如果它第一个操作数为假，就返回第一个操作数，不然返回第二个操作数。
    对于操作符or来说，如果它第一个操作数为真，就返回第一个操作数，不然返回第二个操作数。

]]

--算符运算符
num1=10;
num2=3;
print(num1+num2);  --加，输出：13
print(num1-num2);  --减，输出：7
print(num1*num2);  --乘，输出：30
print(num1/num2);  --除，输出：3.33333
print(num1%num2);  --取余：输出：1
print(num1^num2);  --次幂，输出：1000

--关系运算符
print(tostring(num1>num2));
print(tostring(num1>=num2));
print(tostring(num1<num2));
print(tostring(num1<=num2));
print(tostring(num1==num2));
print(tostring(num1~=num2));

--逻辑运算符
num1=100;
num2=200;
num3=300;
print(num1>num2 and num2<num3);   --与
print(num1>num2 or num2<num3);    --或
print(not (num1>num2));           --非

--需要进行断路规则
print(10 and 20);     --输出：20
print(nil and 80);    --输出：nil
print(false and 66);  --输出：fasle
print(2 or 5);        --输出：2
print(false or 10);  --输出：10

--多重赋值
--若值得个数少于变量的个数，则多余的变量会被赋值为nil
--若值的个数更多的话，多余的值会被忽略掉
num1,num2,num3=10,20,30;
print(num1,num2,num3);


