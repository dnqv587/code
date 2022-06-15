-- 链表
list = nil
int = 0
for line in io.lines() do

    list = {
        next = list, -- 指向下一个节点的引用
        value = line -- value 每行的内容
    }
    int = int + 1
    if int == 1 then
        break
    end
end

local l = list
while l do
    print(l.value)
    l = l.next
end

test = {
    [1] = "1",
    [5] = "5",
    [0] = "0",
    [2] = "2"

}

local t = test
--[[
while t do
    print(t.value)
    test = t.next
end
]]
for i = 0, 5 do
    print(t[i])
end

local l2 = {
    x = 1,
    y = 2
}
print(l2.x, l2.y) -- 等价于l2["x"],l2["y"]

