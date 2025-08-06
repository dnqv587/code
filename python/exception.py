#!/bin/python3

try:
    raise NameError("test")  #抛出异常
except ZeroDivisionError as e: #捕获指定异常
    print(e)
except (NameError,ZeroDivisionError) as ee: #捕获多个异常
    print(ee)
except Exception as e: #捕获全部异常
    print(e)
else: #没有异常时处理
    print('没有异常')
finally: #都会执行
    print('finally')
