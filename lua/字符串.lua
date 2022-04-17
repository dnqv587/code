--第三章，lua字符串的一般规定

--1.定义多行字符串---多用于写页面
strMultiline=[[
    <html>
        <head></head>
        <body>
        <a href="http://www.baidu.com">baidu</a>
        </body>
    </html>
]];

print(strMultiline);

--2.字符串连接
str="123";
print("输出变量 str="..str);   --".."表示字符串连接

--3.字符串的转换
print("2.2"+"30");  --输出：“32.2”；lua中“+”号只代表相加运算符，所以两边自动转换为数值型

print("2.2"+10);    --输出：“12.2”

--4.求字符串长度
strlen="123";
print(#strlen);     --输出：3；“#”代表输出字符串长度
hanzilen="你好";
print(#hanzilen);   --输出：6，在此编码中，每个汉字占3个

--5.字符串中两个重要的转换函数（字符串与其他类型的转换）
--tonumer() 和 totring()
strNum1="888";
num2=666;
res1=strNum1-num2;  --strNum1隐式转换为数值型
print(res1);       --输出222；

res2=tonumber(strNum1)-num2;  --strNum1显式转换为数值型
print(res2);

res3=strNum1..tostring(num2);  --显式转换
res3=strNum1..num2;            --隐式转换
print(res3);

--6.必须使用tostring的场合
--res="汉字"+66;   --出错，无法隐式转化“汉字"为数值型
numArry={1,2,3};
print("表的内容："..tostring(numArry));  --打印内存地址

num=nil;--num变量为空
print("num="..tostring(num));   --输出：num=nil


