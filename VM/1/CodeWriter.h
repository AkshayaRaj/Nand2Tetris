#ifndef CODEWRITER_H
#define CODEWRITER_H
#define MAX_NAMES 1000
#include <string>
#include <fstream>

class CodeWriter{
       
       std::string finalDestination;
       std::string symbolPrefix;
       std::string funcName[MAX_NAMES];
       public:
                static  int count;//the count variable is to prevent repeated symbol names in the assembly code
                static    int callCount;
                static  int currFunc;
              std::fstream ASM;
               CodeWriter(std::string);
              void setFileName(std::string);
              void writeArithmetic(std::string);
              void writePushPop(std::string,std::string,unsigned int );
              void close();
              void writeInit();
              void writeLabel(std::string);
              void writeGoto(std::string);
              void writeIf(std::string);
              void writeCall(std::string,int);
              void writeReturn();
              void writeFunction(std::string,int);
              
              
              };
              
#endif
