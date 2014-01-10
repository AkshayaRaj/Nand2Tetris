 #include "CodeWriter.h"
 #include <iostream>
 #include <conio.h>
 using namespace std;
 
    CodeWriter::CodeWriter(string dest){
                  
                  
                  for(int i=0;i<dest.length();i++){
                          if(dest[i]=='.')
                                          symbolPrefix=dest.substr(0,i+1);
                                          }
                         // cout<<symbolPrefix;
                  // ASM.open(dest.c_str(),ios::out);
                  //cout<<"output created";
                  
                  
                  ASM.open(dest.c_str(),ios::out);
                  count=0;
                  callCount=0;
                  currFunc=0;
                  }
                  
                  int CodeWriter::count;
                  int CodeWriter::callCount;
                  int CodeWriter::currFunc;
                  
              void CodeWriter::setFileName(string fileName){
                   cout<<fileName<<" is being translated to assembly"<<endl;
                   }
    
              void CodeWriter::writeArithmetic(string command){
                  
                   if(command=="add"){
                                      ASM<<"@SP\nAM=M-1\nD=M\nM=0\nA=A-1\nM=M+D\n";
                                      }
                    if(command=="sub")
                                       ASM<<"@SP\nAM=M-1\nD=M\nM=0\nA=A-1\nM=M-D\n";                
                    if(command=="neg")
                                     ASM<<"@SP\nA=M-1\nM=-M\n";
                    if(command=="eq"||command=="gt"||command=="lt"){
                            ASM<<"@SP\nAM=M-1\nD=M\nM=0\nA=A-1\nD=M-D\n@TRUE"<<count<<"\nD;J"<<(char)(command[0]-32)<<(char)(command[1]-32)
                            <<"\n@SP\nA=M-1\nM=0\n@FALSE"<<count<<"\n0;JMP\n(TRUE"<<count<<")\n@SP\nA=M-1\nM=-1\n(FALSE"<<count<<")\n";                
                            count++;
                            }
                     if(command=="and")
                              ASM<<"@SP\nAM=M-1\nD=M\nM=0\nA=A-1\nM=M&D\n";
                     if(command=="or")         
                              ASM<<"@SP\nAM=M-1\nD=M\nM=0\nA=A-1\nM=M|D\n";
                     if(command=="not")
                              ASM<<"@SP\nA=M-1\nM=!M\n";                                                                              
                                                 }
                
                void CodeWriter::writePushPop(string command,string segment,unsigned int index){
                     //handling the push constant segment
                     if (command=="C_PUSH"&& segment=="constant"){
                                             if(index>32767 || index<0){
                                                            cout<<"constant out of range\n Press any key to exit\n";
                                                            abort();
                                                            }
                                             ASM<<"@"<<index<<"\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                                             }
                if(command=="C_PUSH")    {
                                         
                     if(segment=="pointer"){
                                           //cout<<"-push pointer- has not been implemented yet\n";
                                           //getch();
                                           //abort();
                                          ASM<<"@"<<index<<"\nD=A\n@3\nA=A+D\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
                                                       
                                           }
                     if(segment =="local")
                                          ASM<<"@"<<index<<"\nD=A\n@LCL\n";
                     if(segment =="argument")
                                          ASM<<"@"<<index<<"\nD=A\n@ARG\n";
                     if(segment =="this")
                                          ASM<<"@"<<index<<"\nD=A\n@THIS\n";
                     if(segment =="that")
                                          ASM<<"@"<<index<<"\nD=A\n@THAT\n";  
                     if(segment=="static"){
                                           ASM<<"@"<<symbolPrefix<<index<<"\n";
                                           ASM<<"D=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                                           
                                           }
                     }
                     if(segment=="temp"){
                                         if(index>7){
                                                     cout<<"temp exceeded\nPress any key to exit";
                                                     getch();
                                                     abort();
                                                     }
                                         else{ 
                                         int temp_addr=5+index;
                                         if(command=="C_PUSH")
                                                               ASM<<"@"<<temp_addr<<"\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                                         if(command=="C_POP")
                                                             ASM<<"@SP\nAM=M-1\nD=M\nM=0\n@"<<temp_addr<<"\nM=D\n";
                                                             }
                                         }                   
                     if(segment=="pointer"){
                                            if(command=="C_POP"){
                                                                 ASM<<"@3\nD=A\n@"<<index<<"\nA=A+D\nD=A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\nM=0\n@R13\nA=M\nM=D\n";
                                                                
                                                                 }          
                                            }
                        if(command=="C_PUSH" && segment!="pointer" && segment!="temp" && segment!="constant" && segment!="static"){
                                              ASM<<"A=M+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                                              }
                       if(command=="C_POP" && segment!="pointer" && segment!="temp" && segment!="static"&& segment!="this" && segment!="that"){
                                             ASM<<"@"<<index<<"\nD=A\n";
                                               if(segment =="local")
                                                          ASM<<"@LCL\n";
                                             if(segment =="argument")
                                                         ASM<<"@ARG\n";                                                        
                                            ASM<<"A=M+D\nD=A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\nM=0\n@R13\nA=M\nM=D\n";                                                                                                                               
                                                                                    
                                             }
                       if(command=="C_POP" && (segment=="this" || segment=="that")) {
                                           if(segment =="this")
                                                        ASM<<"@THIS\n";
                                            if(segment =="that")
                                                        ASM<<"@THAT\n";   
                                            ASM<<"D=M\n@"<<index<<"\nA=A+D\nD=A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\nM=0\n@R13\nA=M\nM=D\n";
                                                        }
                      if(command=="C_POP" && segment=="static"){
                                                             ASM<<"@SP\nAM=M-1\nD=M\n";
                                                             ASM<<"@"<<symbolPrefix<<index<<"\nM=D\n";
                                                             }
                     }
                     
                     void CodeWriter::writeInit(){
                          ASM<<"@256\nD=A\n@SP\nM=D\n";
                          writeCall("Sys.init",0);
                          }
                     void CodeWriter::writeLabel(string label){
                          if(currFunc)
                                      ASM<<"("<<funcName[currFunc]<<"$"<<label<<")\n";
                          else
                                      ASM<<"("<<label<<")\n";
                              
                          }
                     void CodeWriter::writeGoto(string label){
                         // ASM<<"@"<<funcName[currFunc-1]<<"$"<<label<<"\n"
                          if(currFunc)      
                             ASM<<"@"<<funcName[currFunc]<<"$"<<label<<"\n0;JMP\n";    
                          else                                                                
                              ASM<<"@"<<label<<"\n"
                                 <<"0;JMP\n";
                          }
                     void CodeWriter::writeIf(string label){
                          if(currFunc)
                                       ASM<<"@SP\nAM=M-1\nD=M\nM=0\n@"<<funcName[currFunc]<<"$"<<label<<"\nD;JNE\n";
                          else
                               ASM<<"@SP\nAM=M-1\nD=M\nM=0\n@"<<label<<"\nD;JNE\n";
                          
                          }
                     void CodeWriter::writeCall(string functionName,int numArgs){
                          callCount++;
                          int argOffset=numArgs+5;
                          string pushString="D=M\n@SP\nAM=M+1\nA=A-1\nM=D\n";
                          ASM<<"@RETURN"<<callCount<<"\nD=A\n@SP\nAM=M+1\nA=A-1\nM=D\n";
                          ASM<<"@LCL\n"<<pushString<<"\n";
                          ASM<<"@ARG\n"<<pushString<<"\n";
                          ASM<<"@THIS\n"<<pushString<<"\n";
                          ASM<<"@THAT\n"<<pushString<<"\n";
                          ASM<<"@"<<argOffset<<"\nD=A\n@SP\nD=M-D\n@ARG\nM=D\n";   //ARG=SP-n-5   
                          ASM<<"@SP\nD=M\n@LCL\nM=D\n";
                         // writeGoto(functionName);
                         ASM<<"@"<<functionName<<"\n0;JMP\n";
                          ASM<<"(RETURN"<<callCount<<")\n";
                          
                          }
                     void CodeWriter::writeFunction(string functionName,int nLocals){
                          funcName[++currFunc]=functionName;
                          ASM<<"("<<functionName<<")\n"
                             <<"@"<<nLocals<<"\nD=A\n"
                             <<"@SP\nAM=M+1\nA=A-1\nM=0\n"
                             <<"D=D-1\n@"<<functionName<<"\nD;JGT\n";
                             }
                     void CodeWriter::writeReturn(){
                          ASM<<"@LCL\nD=A\n@R13\nM=D\n"                                         //FRAME=LCL
                             <<"@5\nD=A\n@R13\nA=M-D\nD=M\n@R14\nM=D\n"                         //RET=*(FRAME-5)
                             <<"@SP\nAM=M-1\nD=M\nM=0\n@ARG\nA=M\nM=D\n"                        //*ARG=pop()
                             <<"@ARG\nD=M+1\n@SP\nM=D\n"                                        //SP=ARG+1
                             <<"@1\nD=A\n@R13\nA=M-D\nD=M\n@THAT\nM=D\n"                          //THAT=*(FRAME-1)
                             <<"@2\nD=A\n@R13\nA=M-D\nD=M\n@THIS\nM=D\n"                          //THIS=*(FRAME-2)
                             <<"@3\nD=A\n@R13\nA=M-D\nD=M\n@ARG\nM=D\n"                           //ARG=*(FRAME-3)
                             <<"@4\nD=A\n@R13\nA=M-D\nD=M\n@LCL\nM=D\n"                           //LCL=*(FRAME-4)
                             <<"@R14\nA=M\n0;JMP\n";                                            //goto RET
                             currFunc--;
                             }
                             
                             
                             
                             
              
                void CodeWriter::close(){
                     ASM.close();
                     }     
