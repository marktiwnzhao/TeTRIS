#include "../../generator/include/utils.h"
#include "../../generator/include/ast.h"
#include <fstream>
#include <sstream>
#include <iostream>
int main(int argc,char **argv) { for(int i=1;i<argc;i++){std::ifstream f(argv[i]);std::stringstream s;s<<f.rdbuf(); auto p=parser(s.str());std::cout<<argv[i]<<" "<<(p?"PARSE_OK":"PARSE_FAILED")<<std::endl;if(p)p->deep_delete();} }
