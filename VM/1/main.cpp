#include <cstdlib>
#include <iostream>

#include <conio.h>
#include "CodeWriter.h"



#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <dirent.h>



using namespace std;

int SP=256;
int LCL=0;
int ARG=0;
int THIS=0;



class Parser{
      string currentCommand;
      
      
      public:
             fstream VM;
             Parser(string);
             bool hasMoreCommands();
             void advance();
             string commandType();
             string arg1();
             unsigned int arg2();
             ~Parser(){
                       VM.close();                       
                       }
             };
             
             Parser::Parser(string source){
                              VM.open(source.c_str(),ios::in);
                           //  VM.open("sample.txt",ios::in);
                              }
                              
              bool Parser::hasMoreCommands(){
                                   return(!VM.eof());
                                   }
             void Parser::advance(){
                     if(hasMoreCommands()){
                          getline(VM,currentCommand);
                          }
                     }
             string Parser::commandType()                           {
                    if(currentCommand=="add"|| currentCommand=="sub" ||currentCommand=="neg"|| currentCommand=="eq" 
                        ||currentCommand=="gt"|| currentCommand=="lt" ||currentCommand=="and"|| currentCommand=="or"||
                        currentCommand=="not")
                        return  "C_ARITHMETIC";
                        
                        if(currentCommand[0]=='/' && currentCommand[1]=='/'){
                                                  advance();
                          return "COMMENT";
                          }
                        if(currentCommand.find("push",0)!=string::npos)
                          return "C_PUSH";
                        if(currentCommand.find("pop",0)!=string::npos)
                          return "C_POP";
                        if(currentCommand.find("label",0)!=string::npos)
                          return "C_LABEL";
                        if(currentCommand.find("goto",0)!=string::npos && currentCommand.find("if-goto",0)==string::npos)
                          return "C_GOTO";
                        if(currentCommand.find("if-goto",0)!=string::npos)
                          return "C_IF"; 
                        if(currentCommand.find("function",0)!=string::npos)
                          return "C_FUNCTION"; 
                        if(currentCommand.find("return",0)!=string::npos)
                          return "C_RETURN"; 
                        if(currentCommand.find("call",0)!=string::npos)
                          return "C_CALL"; 
                          
                          return "EXCEPTION";
                                                    
                         }
        string Parser::arg1() {
               int p1=0,p2=0,flag=0;
               
                  if(commandType()=="C_RETURN")
                  return "C_RETURN_EXCEPTION";   
                  
                  if(commandType()=="C_ARITHMETIC"){
                                                                                                       
                                                   
                                                      return currentCommand.substr(0,currentCommand.length());
                                                      }       
                  
               string command=currentCommand;
               for(int i=0;i<currentCommand.length();i++){
                       if(currentCommand[i]==' ' && flag==0){
                       p1=i;
                       flag=1;
                                       i++;
                       } 
                      if(currentCommand[i]==' ' && flag==1){
                         p2=i;
                         break;
                      }
                      
               } 
               //cout<<p1<<p2<<endl;
               
               command=currentCommand.substr(p1+1,p2-p1-1);
               return command;
        }
        
       unsigned int Parser::arg2(){
               int ctr=0,pos;
               for(int i=0;i<currentCommand.length();i++){
                       if(currentCommand[i]==' ')
                         ctr++;
                        if(ctr==2){
                                   pos=i;
                                   break;
                                   }
                        }                      
               string arg2=currentCommand.substr(pos+1,currentCommand.length()-pos-1);
               int arg2_int=atoi(arg2.c_str());
               return arg2_int;
                         
               }
                       
 
 class Parser_factory{
       public:
              Parser *createInstance(string source){
                          return new Parser(source);
                          }
              };
   
                                        
void translate(char* arg){
     int n=0,t;
     char ch;
     char basePath[255]="";
     _fullpath(basePath,NULL,sizeof(basePath));
     cout<<basePath<<"\n";
    
     cout<<"Want to translate a Directory or a single File (D/F) ?\n";
     cin>>ch;
     
     string source=arg;
     string target;     
      int pos=0;
      
      
                                              
                          
     
     if(ch=='F'||ch=='f'){
                 //the following code is to rename the source and the target files accordingly (in case of single vm file)
                          for(int i=0;i<source.length()-1;i++){
                                  if(source[i]=='.')
                                  pos=i;
                                  }
                                  
                                  if(pos==0){
                                             target=source;
                                             target.append(".asm");
                                             source.append(".vm");
                                             }
                                          
                         
                          else{    
                          target=source.substr(0,pos);
                          target.append(".asm");
                          }
                          
                      Parser aks(source);
    CodeWriter out(target);
    out.writeInit();
    while(aks.hasMoreCommands()){
                                 n++;
    aks.advance();
    cout<<aks.commandType()<<endl;
    
   
    
    if(aks.commandType()=="C_ARITHMETIC")
                               out.writeArithmetic(aks.arg1());
                               
    if(aks.commandType()=="C_PUSH"|| aks.commandType()=="C_POP")
                                                            out.writePushPop(aks.commandType(),aks.arg1(),aks.arg2());                                                                                                                       
                                                          
    if(aks.commandType()=="C_LABEL")
                                    out.writeLabel(aks.arg1());
    if(aks.commandType()=="C_GOTO")
                                   out.writeGoto(aks.arg1());
    if(aks.commandType()=="C_IF")
                                 out.writeIf(aks.arg1());
    if(aks.commandType()=="C_FUNCTION")
                                    out.writeFunction(aks.arg1(),aks.arg2());
    if(aks.commandType()=="C_RETURN")
                                   out.writeReturn();
    if(aks.commandType()=="C_CALL")
                                   out.writeCall(aks.arg1(),aks.arg2());
    
                                                                                                                                                                                                                                       
  }
    cout<<"\n"<<n<<"\tCommands\n";     
                 }
                 
     if(ch=='D' || ch=='d'){
                
                CodeWriter out(target);
                DIR *dp;
                struct dirent *ep;
                
                   
                   
                   
                
                }
                
     else{
         cout<<"Invalid option";
         exit(1);
         }
                        
     
     
    
   
     
     }
                  
         

int main(int argc, char *argv[])
{
    
    string param=argv[1];
    
    translate(argv[1]);
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
