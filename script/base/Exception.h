#include <exception>

class field_value_different:public std::exception
{
public:
    field_value_different() noexcept
        :std::exception("字段和值数量不匹配\n",1)
    {

    }
};