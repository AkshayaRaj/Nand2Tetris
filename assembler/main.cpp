
#include <iostream>
#include <fstream>
#include<istream>
#include<bitset>
#include <string.h>
#include<map>


using namespace std;




 class Parser {
       
      string currentCommand;
      string target;
       
            public:
                   
                   fstream Hack,Asm;   
                   Parser(string);
                   Parser();
                   bool hasMoreCommands();    
                   void advance(); 
                   void print(string);
                   void print();
                   string commandType();
                   string symbol();
                   string dest();
                   string comp();
                   string jump();
                   string targetFile();
                   void reset();
                   ~Parser(){
                                  Asm.close();
                                  Hack.close();
                                  }
             
       
       }; 
          
       
 Parser::Parser(string source){
                          int pos=0;
                          for(int i=0;i<source.length()-1;i++){
                                  if(source[i]=='.')
                                  pos=i;
                                  }
                                  
                                  if(pos==0){
                                             target=source;
                                             target.append(".hack");
                                             source.append(".asm");
                                             }
                                          
                          Asm.open(source.c_str(),ios::in);
                          if(pos){    
                          target=source.substr(0,pos);
                          target.append(".hack");
                          }
                                              
                          Hack.open(target.c_str(),ios::out);      
                          
                                  
                  }
                  
                  
                  void Parser::print(){
                       cout<<currentCommand;
                       }
                  
                  
                  
           Parser::Parser(){                      
           Hack.open("out.txt",ios::out);   
           Asm.open("sample.txt",ios::in);   
                }
                
               string Parser::targetFile(){
                                     return target;
                                     }
                
       
       bool Parser::hasMoreCommands(){
               
               return (!Asm.eof());
              
               }
       
   void Parser::advance(){
                      if(hasMoreCommands())
                          getline(Asm,currentCommand);   
                                        
                                           
                        }
    void Parser::reset(){
         Asm.clear();
         Asm.seekg(0,Asm.beg);         
         }
                        
   void Parser::print(string p){
        Hack<<p<<"\n";
        }
                             
       
   string Parser::commandType(){
          
          int f1=0,f2=0,l_flag=0,l_flag1=0,l_flag2=0;
          for(int i=0;i<currentCommand.length();i++){
                  if(currentCommand[i]=='=')
                     f1=1;
                  if(currentCommand[i]==';')
                     f2=1;
                  if(currentCommand[i]=='(')
                    l_flag1=1;
                  if(currentCommand[i]==')')
                    l_flag2=1;  
                     
                  }
              if(l_flag1*l_flag2)
                l_flag=1;
                  
          if(currentCommand[0]=='/' && currentCommand[1]=='/')
            return("Comment");       
          if(currentCommand[0]=='@')
            return ("A_COMMAND");
          else if(f1==1 || f2==1)
             return("C_COMMAND");
          else if(l_flag==1)
             return("L_COMMAND");
          else return ("INVALID");
          
          
          }
          
         string Parser::symbol(){
                if(!commandType().compare("A_COMMAND")){
                                                                   
                                                                                     
                                                      string A_Command=currentCommand.substr(1,currentCommand.length()-1);     
                                                      return A_Command;              
                }
                
                 if(commandType()=="L_COMMAND"){
                                                      string L_Command=currentCommand.substr(1,currentCommand.length()-2);     
                                                      return L_Command;           
                }
                
                return "EXCEPTION";
   
                                                      }                  
                                                      
            string Parser::dest(){
                            string dest;
                            int a=99,b=99;
                            int t,p=0;
                            
                            for(int p=0;p<currentCommand.length();p++){
                           if(currentCommand[p]=='=')
                              a=p;
                            if(currentCommand[p]==';')
                               b=p;
                               
                               }
                               
                              if(a!=99 &&b!=99){
                                       dest=currentCommand.substr(0,a);
                                       return dest;
                                       }
                                       
                                if(a!=99 && b==99){
                                         dest=currentCommand.substr(0,a);
                                         return dest;
                                         }
                                if(a==99 && b!=99){
                                         dest="NULL";
                                         return dest;
                                         }
            }                                      
            
            string Parser::comp(){
                         string comp;
                   int a=99,b=99;
                   for(int p=0;p<currentCommand.length();p++){
                           if(currentCommand[p]=='=')
                              a=p;
                            if(currentCommand[p]==';')
                               b=p;
                                             }  
                                                         
                                if(a!=99 && b!=99){                             
                               comp=currentCommand.substr(a+1,b-a-1);
                               return comp;
                               }
                               if(a!=99 && b==99){
                                        comp=currentCommand.substr(a+1,currentCommand.length()-1);
                                        return comp;
                                        }
                                if(a==99 && b!=99){
                                         comp=currentCommand.substr(0,b);
                                         return comp;
                                         }
                               
                               }
                               
                             
         string Parser::jump(){
                string jump;
                int a=99,b=99;
                for(int p=0;p<currentCommand.length();p++){
                           if(currentCommand[p]=='=')
                              a=p;
                            if(currentCommand[p]==';')
                               b=p;
                               }
                  if(a!=99 && b!=99){                             
                               jump=currentCommand.substr(b+1,3);
                               return jump;
                               }
                               if(a!=99 && b==99){
                                        jump="NULL";
                                        return jump;
                                        }
                                if(a==99 && b!=99){
                                         jump=currentCommand.substr(b+1,3);
                                         return jump;
                                         }
                               
                               }
                               
   class Code{
         public:
                string dest(string);
                string comp(string);
                string jump(string);     
                    
         };
         
         string Code::dest(string dest){
                string dest_code="000";
                for(int q=0;q<dest.length();q++){
                                    if(dest[q]=='A')
                                       dest_code[0]='1';
                                    if(dest[q]=='D')
                                       dest_code[1]='1';
                                    if(dest[q]=='M')
                                       dest_code[2]='1';
                                     }    
                return dest_code;
                }
                
         string Code::comp(string comp)       {
                string comp_code="0000000";
                
                //top three lines are handled
                if(comp=="0")
                  return "0101010";
                if(comp=="1")
                  return "0111111";
                if(comp=="-1")
                  return "0111010";
                    
                    if(comp.find("D")==string::npos)
                      comp_code[1]='1';
                      
                      if(comp.find("M")==string::npos && comp.find("A")==string::npos )
                      comp_code[3]='1';
                      
                      //now lets figure out if a=0 or a=1
                      //i'll use just one condition: if M is found,then a=1
                      if(comp.find("M")!=string::npos)
                        comp_code[0]='1';
                        
                        //now lets figure out the condition for c5. 
                        //if + or - occurs, assign c5=1
                        if((comp.find("+")!=string::npos || comp.find("-")!=string::npos ))
                         comp_code[5]='1';
                         
                         //handling c2, just permutations..
                         if(comp_code[1]=='1')
                           comp_code[2]='1';
                         if(comp=="D+1" || comp=="D-A" || comp=="D-M" || comp=="D|A" ||comp=="D|M" ||
                         comp=="1+D" || comp=="-A+D" || comp=="-M+D" || comp=="A|D" ||comp=="M|D")
                       
                          comp_code[2]='1';
                          
                          //now handling c4, just permutations..
                          if(comp_code[3]=='1')
                           comp_code[4]='1';
                           if(comp=="A+1" || comp=="M+1" || comp=="A-D" || comp=="M-D" ||comp=="D|M" ||comp=="D|A"||
                           comp=="1+A" || comp=="1+M" || comp=="-D+A" || comp=="-D+M" ||comp=="M|D" ||comp=="A|D")
                            comp_code[4]='1';
                            
                            //handling c6 is just permutations
                            if(comp=="D" || comp=="A" || comp=="M" || comp=="D-1" ||comp=="A-1" ||comp=="M-1"||
                            comp=="D+A" || comp=="D+M" || comp=="D&A" || comp=="D&M" ||
                            comp=="-1+D" || comp=="-1+A" || comp=="-1+M" || comp=="A+D" ||comp=="M+D" ||comp=="A&D" ||comp=="M&D")
                              comp_code[6]='0';
                               else
                               comp_code[6]='1';
                
                    return comp_code;
                    
                
                }
           
          string Code::jump(string jump){
                 //string  jump_code="000";
                 if(jump=="NULL")
                 return "000";
                 if(jump=="JGT")
                 return "001";
                 if(jump=="JEQ")
                 return "010";
                 if(jump=="JGE")
                 return "011";
                 if(jump=="JLT")
                 return "100";
                 if(jump=="JNE")
                 return "101";
                 if(jump=="JLE")
                 return "110";
                 if(jump=="JMP")
                 return "111"; 
                 
                 return "JUMP EXCEPTION";
                 }
                 

 class SymbolTable{
       map<string,int>list;
       public:
              SymbolTable();
              void addEntry(string,int);
              bool contains(string);      
              int getAddress(string);
              int getSize();
              
              };
              
              SymbolTable::SymbolTable(){
                                 list["R0"]=0;
                                 list["R1"]=1;
                                 list["R2"]=2;
                                 list["R3"]=3;
                                 list["R4"]=4;
                                 list["R5"]=5;
                                 list["R6"]=6;
                                 list["R7"]=7;
                                 list["R8"]=8;
                                 list["R9"]=9;
                                 list["R10"]=10;
                                 list["R11"]=11;
                                 list["R12"]=12;
                                 list["R13"]=13;
                                 list["R14"]=14;
                                 list["R15"]=15;
                                 list["SP"]=0;
                                 list["LCL"]=1;
                                 list["ARG"]=2;
                                 list["THIS"]=3;
                                 list["THAT"]=4;
                                 list["SCREEN"]=16384;
                                 list["KBD"]=24576;
                                         
                                         }
           void SymbolTable::addEntry(string name,int address)  {
                list[name]=address;
                }
              
              bool SymbolTable::contains(string name){
                      if(name=="SP" || name=="R0")
                        return true;
                      if(list[name]==0)
                        return false;
                       return true;   
                       }               
                   
              int SymbolTable::getAddress(string name)                      {
                  return list[name];
                  }
                
              int SymbolTable::getSize(){
                  return list.size();
                  }    
              
       
int main(int argc,     char *argv[])
{
    
     if(argc==1){
                 cout<<"USAGE: "<<argv[0]<<" <inputfile> "<<endl;
                 system("PAUSE");
                 return EXIT_FAILURE;
                 
                 }
    
    string param=argv[1];
    cout<<"assembling "<<argv[1]<<endl;

    
        
   // char * filename="out.hack";
    string c="111";
    int base_adr=16; //last predefined symbol is at RAM 15
 
    int rom_firstpass=0;// increments by 1 when a C or A instruction is encountered.


    Parser file(param);
    Code aks;
    SymbolTable sym;
    
   while(file.hasMoreCommands()){
                                  file.advance();
                                   if(file.commandType()=="L_COMMAND"){
                                      sym.addEntry(file.symbol(),rom_firstpass);
                                         }
                                      if(file.commandType()=="A_COMMAND" || file.commandType()=="C_COMMAND")   
                                         rom_firstpass++;
                                   }    
               cout<<"..."<<endl;
               file.reset();  
    
    while(file.hasMoreCommands()){
                                  
    
    file.advance();

    if(file.commandType()=="Comment")
      file.advance();
    if(file.commandType()=="A_COMMAND"){
                                        int x_flag=0;
                                        int adr ;
                                        
                                    for(int i=0;i<file.symbol().length();i++){
                                            if(!isdigit(file.symbol()[i])){
                                                                          x_flag=1;
                                                                          break;
                                                                          }
                                                          }   
                                                          
                                                       if(x_flag==1){ //the command is like @KBD or maybe undefined like @i
                                                       
                                                       
                                                         
                                                            if(sym.contains(file.symbol())==true){  //if the symbol is predefined eg. @KBD lookup symbol table                                                            
                                                                adr=sym.getAddress(file.symbol());
                                                                file.print(bitset<16>(adr).to_string<char,char_traits<char>,allocator<char> >());
                                                                
                                                                }
                                                            
                                                             if(sym.contains(file.symbol())==false) //add the symbol to the symbol table and allocate RAM to it
                                                                {                                                                                                 
                                                                  sym.addEntry(file.symbol(),base_adr);                                                                  
                                                                   file.print(bitset<16>(base_adr).to_string<char,char_traits<char>,allocator<char> >());
                                                                   base_adr++;
                                                                   } 
                                                                                                                                            
                                                          
                                                          }
                                                                                  
                                              
                                    if(x_flag==0){       
                                    string a = bitset< 16 >( atoi(file.symbol().c_str()) ).to_string<char,char_traits<char>,allocator<char> >();
                                         file.print(a);
                                         }
                                        // cout<<a<<endl;
                                        
                                  }
   if(file.commandType()=="C_COMMAND"){
                                       c.append(aks.comp(file.comp()));
                                       c.append(aks.dest(file.dest()));
                                       c.append(aks.jump(file.jump()));
                                       file.print(c);
                                     //  cout<<c<<endl;
                                        c="111";
                                        
                                       }
                                                                            
/*
 
    cout<<file.commandType()<<endl;
    cout<<file.dest()<<" "<<file.comp()<<file.jump()<<"hi"<<endl;
    cout<<"Dest mnemonic: "<<aks.dest(file.dest())<<endl;
    cout<<"Comp mnemonic: "<<aks.comp(file.comp())<<endl;
    cout<<"Jump mnemonic: "<<aks.jump(file.jump())<<endl;
    cout<<endl;
   
    
  */
    
    
    
       
}
       
    cout<<file.targetFile()<<endl;
    system("PAUSE");
    return EXIT_SUCCESS;
}







       
       
       
       
       
