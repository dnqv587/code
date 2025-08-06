#!/bin/python3

import threading
import time
from typing import Union

#闭包
def outer(log):
    def inner(msg):
        print(f"<{log}>{msg}<{log}>")
    return inner


fn1=outer("测试1")
fn1("消息")
fn1("你好")

fn2=outer("世界")
fn2("hello")
fn2("你好")

#多线程
def sing(msg):
    while True:
        print(f"sing:{msg}")
        time.sleep(1)
    
def dance(msg):
    while True:
        print(f"dancing:{msg}")
        time.sleep(1)

thread_sing=threading.Thread(
    target=sing,
    args=("music",)
)

thread_dance=threading.Thread(
    target=dance,
    kwargs={"msg":"dance"}
)

#thread_dance.start()
#thread_sing.start()

#递归

def fibonacci(num:int):
    if(num==0):
        return 0
    if(num==1):
        return 1
    return fibonacci(num-1)+fibonacci(num-2)

fib=fibonacci(20)
print(f"fibonacci={fib}")

