#!/bin/python3

from pymysql import Connection

#链接对象
conn = Connection(
    host='localhost',
    port=3306,
    user='dai',
    password='qvw1418546'
    # autocommit=true   #自动提交事务
)

#获取游标
cursor=conn.cursor()
#选择数据库
conn.select_db("test")
#使用游标对象，执行sql语句
#cursor.execute("create table test_py(id int)")
cursor.execute("select * from employee")
result:tuple=cursor.fetchall()
for row in result:
    print(row)

cursor.execute("insert into employee(id,name,sex,part_id) values(0005,'lilis','1',4)")
conn.commit() #提交事务

#打印信息
#print(conn.get_server_info())

#关闭数据库
conn.close()

