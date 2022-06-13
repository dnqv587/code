-- 链表
list = nil
int = 0
for line in io.lines() do

    list = {
        next = list, -- 指向下一个节点的引用
        value = line -- value 每行的内容
    }
    int = int + 1
    if int == 10 then
        break
    end
end

local l = list
while l do
    print(l.value)
    l = l.next
end

function func()
    return 123, "abc", "hello"
end

ret = func().unpack
local r = ret
count = 1
while r do
    print(r[count])
    count = count + 1
    r = r.next
end

