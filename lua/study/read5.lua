func = loadfile("read4.lua", "t")
str = func()
print(str)
s = dofile("read4.lua")
print(s)
-- 多重返回值
function doubleRet()
    return "a", "b"
end
x, y, z = 10, doubleRet() -- x=10,y="a",z="b"
print(x, y, z)
t = {doubleRet} -- t={"a","b"}

function DtoH(d)
    if d == 0 then
        return
    end
    d = d / 2
    if d == math.tointeger(d) then
        return 0, DtoH(math.ceil(d))
    else
        return 1, DtoH(math.ceil(d) - 1)
    end
end

f = {DtoH(99999999)}
for k, iter in ipairs(f) do
    print(iter)
end
