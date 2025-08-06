#!/bin/python3
import json

data=[{"name":"张大山","age":11},{"name":"王大锤","age":18},{"name":"赵小虎","age":19}]

json_str=json.dumps(data,ensure_ascii=False) #转换成json
print(json_str)

pydata=json.loads(json_str)

print(pydata)
