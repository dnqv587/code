#include "insert/generateSql.h"


void genesql(generateSql& sql)
{
    sql.geneInsertSql("label", {"a", "b", "c"}, {"1", "2", "3"}).repeat("a", 10, 20);
    sql.output();
}

int main(int argc,char* argv)
{

    if (argc == 2)
	{
        generateSql sql(argv[1], "Sql.txt");
        genesql(sql);
    }
	else if (argc == 3)
	{
		generateSql sql(argv[1], argv[2]);
        genesql(sql);

	}
	else
	{
		generateSql sql("obj.txt", "sql.txt");
        genesql(sql);

	}

#ifdef _WIN32
        system("pause");
#endif 
    return 0;
}