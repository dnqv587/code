#include "generateSql.h"
#include <iostream>

generateSql::generateSql(std::string source, std::string obj)
    :_in(source),
    _out(obj),
    _insert("insert into %s(%s) values(%s);")
{

}

generateSql::~generateSql()
{

}

continueGene* generateSql::geneInsertSql(std::string label,std::vector<std::string>& fields,std::vector<std::string>& values)
{
    std::string sql;
    
    if(field.size()!=value.size())
    {
        std::cerr << "字段和值数量不匹配\n";
        throw field_value_different();
        return "字段和值数量不匹配\n";
    }
    
    std::string field;
    for(const auto& str:fields)
    {
        field.append(str);
        field.push_back(',');
    }
    std::string value;
    for(const auto& str:values)
    {
        value.append(str);
        value.push_back(',');
    }

    sql.resize(_insert.size + label.size + field.size + value.size);
    sprintf(sql.c_str(), _insert.c_str(), label.c_str(), field.c_str(), value.c_str());

    _outStr.emplace_back(sql);
    return continueGene(label,fields,values,this);
}

void generateSql::output()
{
    for(const auto str:_outStr)
    {
        _out << str;
    }
    _out.flush();
}
