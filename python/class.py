#!/bin/python3
from typing import Union

class Person:
    name=None
    age=None
    __id=None #私有成员变量，前面加__

#构造方法
    def __init__(self,name,age):
        self.name=name
        self.age=age


    def age(self):
        return self.age

    def name(self):
        return self.name

    def say(self):
        print(f"name={self.name},age={self.age}")
    
    def __id(self):  #私有成员方法，前面加__
        return self.__id

#魔术方法
    def __str__(self): #强转为str的方法
        return f"class:name={self.name},age={self.age}"

    def __lt__(self,other):  #比较方法:<,>
        return self.age<other.age
    
    def __le__(self,other):  #比较方法:<=,>=
        return self.age<=other.age

    def __eq__(self,other):  #比较方法:==
        return self.age==other.age



p=Person("王大锤",18)
p.say()

p2=Person("李四",32)

print(p>p2)
print(p<=p2)
print(p==p2)

#p.__id=12 #不报错，但无效
#p.__id()  #报错

class Base:
    __id=None
    __name=None

    def printf(self):
        print("this is base,id={self.__id},name={self.__name}")

#继承
class Derived(Base):
    #pass 若无需内容，可跳过
    def __init__(self,id,name):
        self.__id=id
        self.__name=name

    def printf(self):
        print(f"this is derived id={self.__id},name={self.__name}")

    def BasePrint(self):
        Base.printf(self) #调用基类方法
        #方式2
        super().printf()

dd = Derived(10,'test')
dd.printf()

dd.BasePrint()

#类型注解
var_1:int=10
var_2:str='hello world'
var_3:list=[1,2,3]
var_4:dict={"name":"jac"}
var_5:list[str,int,bool]=['hello',12,False]  #详细注解
var_6=5 # type: int

def add(x:int,y:int)->int:
    return x+y

##联合类型注解
list_1:list[Union[int,str]]=[99,'one',12,'two',666]

def max(x:Union[int,float],y:Union[int,float])->Union[int,float]:
    if x>y:
        return x
    else:
        return y

#多态
class Animal: #抽象类
    def speak(self): #抽象方法
        pass

class Cat(Animal):
    def speak(self):
        print('miao')

class Dog(Animal):
    def speak(self):
        print('wang')

def make_say(animal:Animal):
    animal.speak()

cat=Cat()
dog=Dog()
make_say(dog)
make_say(cat)
