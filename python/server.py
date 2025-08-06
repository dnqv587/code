#!/bin/python3

import socket

socket_server=socket.socket()
socket_server.bind(("localhost",8800))
socket_server.listen(1024)
conn,address=socket_server.accept()
print(f"客户端连接：{address}")
while True:
    data=conn.recv(1024,).decode("UTF-8")
    print(f"接受消息:{data}")
    msg=input("回复：").encode("UTF-8")
    if msg=='exit':
        break
    conn.send(msg)

conn.close()
socket_server.close()


