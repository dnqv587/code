#!/bin/python3
#打开
fd=open('./basic.py','r',encoding='UTF-8')
#读取
word10=fd.read(10) #读取10个组合
word_offset10=fd.read()#读取偏移指针后全部数据
print(f'10 word={word10}\n,all={word_offset10}')               
#读取行
line1=fd.readline()
for line in fd:
    print(line)

#关闭文件
fd.close()

#with open -- 结束后自动关闭文件
with open("./basic.py",'r',encoding='UTF-8') as fd:
    for line in fd:
        print(line)

wfd=open('./test.txt','w',encoding='UTF-8')
#写入
wfd.write('hello world')
#刷新
wfd.flush()

wfd.close()

#追加操作
afd=open('./test.txt','a',encoding='UTF-8')
afd.write('\n')
afd.write('你好，世界')
afd.flush()
afd.close()
