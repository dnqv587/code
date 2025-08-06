#!/bin/python3

# hello world
print("hello world!")

# 变量类型

## 1、数字
### int -- 有符号整形 --3.0之后，int类型数据溢出后自动转为long类型
int_case = 21
### long -- 长整形 -- 2.2之后被移除
#long_case = 12345L
### float -- 浮点型
float_case = 3.1415926
### complex -- 复数
complex_case = 3.14j

print(type(int_case))
print(type(float_case))
print(type(complex_case))

## 2、字符串
### string -- 字符串
str_case = 'hello world!'
print(str_case)
### 下标
#### 从左到右索引默认从0开始，依次递增
#### 从右到左索引默认从-1开始，依次递减
#### 'hello'-> h  e  l  l  o
####           0  1  2  3  4
####          -5 -4 -3 -2 -1

### 截取
print(str_case[0:5])
### 连接
print(str_case[0:5] + '-world')
### 重复操作
print(str_case[0:5] * 2)

## 3、列表
list_case = ['world',3.14,99,['1',2,3.0],'end']
print(list_case) ## 输出完整列表
print(list_case[0]) ##输出第一个元素
print(list_case[1:4]) ##输出第二到第四个元素
print(list_case[1:])  ## 输出第二至最后的元素

## 4、元组
### 元组不能二次赋值，相当于只读列表
tuple_case = ('begin',123,3.1415,['1',2,3.0],'end')

# 类型转换
is_int = 999
is_float = 3.1415
is_str = "1234567"
to_int = int(is_str)
print(to_int,',type=',type(to_int))
to_str=str(is_float)
print(to_str,',type=',type(to_str))
to_float=float(is_str)
print(to_float,',type=',type(to_float))

#运算符
## + 加
## - 减
## * 乘
## / 除
## // 取整除
## %  取余
## ** 指数

#字符串拼接
print('hello' + ' world')

#字符串格式化
print("%s%d%s%f"%('int=',123,',float=',3.14))

#精度控制
print('%4.3f'%123.12345) #4.3f表示4位整数长度，3位小数长度

#字符串快速格式化
print(f'string={'hello world'},int={123},float={3.1415}')

#数据输入
name = input('input your name:\n')
print('name is=%s'%name)

#布尔类型
bool_type = True
bool_type2 = False

#if语句
age = int(input('input age=')  )
if age >= 18:
    print('adult')

#if else 语句
if age >=18:
    print('adult')
else:
    print('no adult')

#if elif else 语句
if age >=18:
    print('adult')
elif age <12:
    print('chidreen')
else:
    print('no adult')   

# while循环
count = 0
while count < 10:
    print('count=%d\n'%count)
    count += 1

#for循环
# for 临时变量 in 序列类型
# 序列类型：字符串、列表、元组 等
for_test = 'hello world'
for i in for_test:
    print(i)

#range语句
range(5) # --->[0,1,2,3,4]
range(1,3) # --->[1,2]
range(1,10,3) #-->[1,4,7]

for i in range(1,10,2):
    print(i)

#continue break
for i in range(1,100):
    if i%2==0:
        continue;
    else:
        print(i)
    if i >50:
        break

#函数
def str_len(data):
    count = 0
    for i in data:
        count += 1
    return count

func_test = str_len('hello world')
print(f'str len={func_test}')

#全局变量
def test():
    global num #声明num位全局变量
    num = 100
    return num

print(test())
print(num)

#多返回值
def miti_ret():
    return 1,'2',3.14
one,tow,pi = miti_ret()
print('多返回值：%d,%s,%.2f'%(one,tow,pi))

#缺省参数
def param_test(name,age,sex='male'):
    print(f"name={name},age={age},sex={sex}")
param_test('dai',26)

#容器
##列表、元组、字符串、集合、字典

#list - 列表
none_list = []
none_list1=list()
list_case1=['one','two','three']
list_case2=['one',2,3.14]
print('列表:')
print(list_case1)
print(list_case2)
#下标
print(list_case1[0])
print(list_case1[-1])
#方法
##获取元素的下标  
get_index = list_case1.index('two') 
print('获取的元素下标：%d'%get_index)
#修改元素
list_case1[0]='1'
print(f'修改后的值{list_case1[0]}')
#插入元素
list_case1.insert(1,'test')
list_case1.insert(2,'test')
print(f'修改后的列表,{list_case1}')
#追加元素
list_case1.append('element')
print(f'追加后的列表：{list_case1}')
#批量追加元素
list_case1.extend([9,8,7,6])
print(f'批量追加后的列表，{list_case1}')
#删除元素
del list_case1[0] #方式1
print(f'方式1删除后列表，{list_case1}')
pop_element = list_case1.pop(-1)
print(f'方式2删除后列表：{list_case1},删除的元素:{pop_element}')
#删除匹配项
list_case1.remove('test') #删除匹配的第一个元素
print(f'匹配项删除后列表,{list_case1}')
#清空
list_case1.clear()
#统计个数
list_case1.extend([1,2,3,4,5,1,2,1])
print(f'统计个数为：{list_case1.count(1)}')
#统计元素个数
list_len=len(list_case1)
print(f'元素个数为:{list_len}')

#元组--定义后不可修改
none_tuple=()
none_tuple=tuple()
tuple_case1=('one','two','three',4,5,3.1415)
print(tuple_case1)

#字符串
mystr='one two three four'
two_idx = mystr.index('two')
new_str=mystr.replace('two','2')
print(f'index={two_idx},new str={new_str},old_str={mystr}')
split_str=mystr.split(' ')
print(split_str)
my_str='   12one21   '
strip_str=my_str.strip()
strip_str1=strip_str.strip('12')
print(f'strip()={strip_str},strip(\'12\')={strip_str1}')
str_count=my_str.count('2')
str_len=len(my_str)

#切片
mylist=[1,2,3,4,5,6,7,8,9]
cut_list=mylist[1:4]
cut_list1=mylist[:]
cut_list2=mylist[::2]
cut_list3=mylist[::-1]

#集合 set
none_set=set()
set_case={'1','2','3','4'}
set_case.add('5')
set_case.remove('2')
element = set_case.pop()
set_case.clear()
set1={1,2,3,4,5}
set2={1,8,9}
set1.difference(set2)
set1.difference_update(set2)
set1.union(set2)
len(set1)

#字典 -dict
none_dict=dict()
dict_case={'name':'dai','sex':'male','age':26}
print(dict_case)
print(dict_case['name'])

# 关键字传参

def user_info(name,age,gender):
    print(f'name={name},age={age},gender={gender}')

user_info(gender='male',name='halo',age=18)

user_info('halo',gender='male',age=18)

#不定长传参
## 位置传递  -- 转换为元组
def arg_info(*args):
    print(args) 
arg_info('tom',18,'female')

##关键字传递 -- 转换为字典
def kvargs_info(**args):
    print(args)
kvargs_info(name='Tom',age=18,gender='female')

#函数作为参数
def func_arg(func):
    func(name='jinx',age=99,id=12)
func_arg(kvargs_info)

#lambda表达式 -- 一行代码
func_arg(lambda name,age,id:
         print(f'x={name},y={age}z={id}'))
lam_func = lambda **args:print(args)
lam_func(name='12')
