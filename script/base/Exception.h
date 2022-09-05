#include <exception>

namespace std
{
class field_value_different
    : public exception
{
public:

    field_value_different() noexcept
    {

    }

     _NODISCARD char const* what() const override 
    {
        return "字段和值数量不匹配\n";
    }
};
}