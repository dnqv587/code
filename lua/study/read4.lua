x = 1
y = 2
x, y = y, x -- 交换x与y
print(x, y)

do -- 程序块

end

-- if语句
if false then

elseif false then

elseif false then

else

end

-- while语句
while false do

end

-- repeat语句
repeat

until true -- 结束条件，条件为真结束循环

-- 数值型for
for var = 1, 10, 1 do -- var从1到10，步长1

end

-- 泛型for
local tb = {"a", "b", "c", "d", "e"}
for i, t in ipairs(tb) do -- ipairs遍历数组的迭代器函数
    print(i, t) -- i为索引值，t为内容
end

function func()
    do
        return
    end -- return和break必须为一个块的最后一条语句，所以当需要在块的中间使用时可以用do end
end

return 123
