//
// Created by dainq45972 on 2022/10/18.
//

#ifndef GENENUM__GENENUM_H_
#define GENENUM__GENENUM_H_

#include <fstream>
#include <stdint.h>

class geneNum
{
 public:
	geneNum(std::string filename);

	~geneNum();

	void generate(int64_t begin,int64_t end,int n);

 private:
	std::ofstream _out;

};

#endif //GENENUM__GENENUM_H_
