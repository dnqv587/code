print('hello world!')
function Fact(n)
    if n == 0 then
        return 1
    else
        return n + Fact(n - 1)
    end
end

local a=io.read("n")
print(Fact(a))
print(type(a))

