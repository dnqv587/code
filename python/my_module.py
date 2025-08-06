
__all__=['add'] #使用‘from <moudle> import *’,限制导入的功能

def add(a,b):
    return a+b

def max(a,b):
    if a > b:
        return a
    else:
        return b

def printf(*args):
    print(args)
