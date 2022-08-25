#pragma once
#include "../base/SqlTools.h"
#include <string>
#include <iostream>
#include <vector>
#include <stdint.h>

class generateSql:protected SqlTools
{
public:
    generateSql(std::string source, std::string obj);

    ~generateSql();

    continueGene* geneInsertSql(std::string label,std::vector<std::string>& field,std::vector<std::string>& value);

    void repeat()

    void output();

    class continueGene
    {
    public:
        continueGene(std::string label,std::vector<std::string>& field,std::vector<std::string>& value,generateSql* generateSql)
            :_label(label),
            _field(field),
            _value(value),
            _pos(0),
            _generateSql(generateSql)
        {

        }
        continueGene& repeat(std::string& field,int32_t start,int32_t end)
        {
            for (std::vector<std::string>::iterator iter = _field.begin(); iter != _field.end();++iter)
            {
                if(*iter==field)
                {
                    break;
                }
                ++_pos;
            }
            for(int32_t i=)
            _generateSql.geneInsertSql()
        }

        operator std::string()
        {
            
        }
    private:
        std::string &_label;
        std::vector<std::string> &_field;
        std::vector<std::string> &_value;
        int _pos;
        generateSql* _generateSql;
    };

private:
    std::string _insert;
    std::string _update;
    std::vector<std::string> _outStr;
};