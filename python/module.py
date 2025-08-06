#!/bin/python3
import time #导出全部功能

print('sleep')
time.sleep(1)
print('weak')

from time import sleep #导入具体功能名
sleep(1)

#自定义模块导入
import my_module
add_val=my_module.add(1,2)
print(add_val)
max_val=my_module.max(1,2)
print(max_val)

if __name__ =='__main__': #判断是否被运行
    print('被运行')

#自定义包
import package.module1
import package.module2
#from package import module1
#from package.module1 import add

val = package.module1.add(2,3)
print(val)

#第三方包
