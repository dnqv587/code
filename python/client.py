#!/bin/python3

import socket

socket_cli=socket.socket()
socket_cli.connect(("localhost",8800))
socket_cli.send("hello".encode("UTF-8"))
msg=socket_cli.recv(1024)
print(f"收到消息：{msg.decode('UTF-8')}")

socket_cli.close()
