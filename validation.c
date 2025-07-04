#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "varification.h" 
#include "translation.h"
#define True 1
#define False 0
#define isColon(x) (x == ':')
#define openparen(x) (x == '(')
#define closeparen(x) (x == ')')
#define isdol(x) (x == '$')
#define isArrow(x) (strcmp(x, "<-") == 0)
#define isClose(x) (strcmp(x, ")") == 0)
#define isAlpha(x) ((x | 0x20) >= 'a' && (x | 0x20) <= 'z')  
#define isNum(x) (x >= '0' && x <= '9') 
#define isAlphaNum(x) (isAlpha(x) || isNum(x))
#define isNewline(x) (x == '\n')  
#define isplus(x) (x == '+')
#define isMinus(x) (x == '-')
#define ismul(x) (x == '*')
#define isZero(x) (x == '0')
#define isCross(x) (x == 'x' ||x=='X')
#define isHash(x) (x == '#')
#define MAX_size 100
#define MAX_io_size 2000
int is_valid_sbrname( char *word);
int chk_valid_g64_reg( char *word);
int chk_valid_g32_reg(char *word);
int chk_is_imme( char *imme);
int is_valid_label( char *word);

void addSubroutine(char *name, int isDefined, int isCalled);
void addLabelToSbr(char *sbrName, char *labelName, int isDefine,int iscall);
typedef unsigned int Bool;
typedef struct TA1state * TA1state;
typedef enum Wordtype Wordtype;
enum Wordtype{
LABEL,Sbrname,Parameter,Openparan,Closeparan,
Imm,Imm8,R32,R64,MEM,Arw,PArw,SArw,MArw,LS,RS,
Plus,Minus,Mul,BR,RET,FP,Newline,EOT,Unknown,
Return,LEQ,GEQ,LT,GT,DEQ,NEQ,CMPBranch
};
struct TA1state {
    char iofile[MAX_io_size];
    char nw[MAX_size];
    char sbrName[MAX_size];
    char labelName[MAX_size];
    int temp;
    Wordtype  instructionType;
};
void TA1statebuild(struct TA1state *t, const char *input) {

    strncpy(t->iofile, input, sizeof(t->iofile) - 1);
    t->iofile[sizeof(t->iofile) - 1] = '\0';
    t->temp = 0;
}
void Nextword(struct TA1state *t)
{

    int j = 0;
    while (t->iofile[t->temp] != '\0' && t->iofile[t->temp]==' ')
    {
        t->temp+=1;
        if(t->iofile[t->temp]=='\0')
        {
        t->nw[0]='\0';
        return;
        }
    }
    while (t->iofile[t->temp] != '\0' && t->iofile[t->temp]!=' ')
    {
        t->nw[j++] = t->iofile[t->temp++];
    }
    t->nw[j] = '\0';
}
int is_valid_sbrname(char *nw) {

        if (isdol(*(nw))) {
            char *ptr = nw +1;
            if (isAlpha(*ptr)) {
                ptr++;
                while (*ptr != '\0') {
                    if (isAlphaNum(*ptr)) {
                        ptr++;
                    } else {
                        break;
                    }
                }
                return 1;
            }
        }
        return 0;
   }
   
   int chk_valid_g64_reg(char *t) {
    if (t[0] == 'X'||t[0]=='x') {
        if (t[1] >= '0' && t[1] <= '9') { 
            if (t[1] == '3' && (t[2] == '0' || t[2] == '1') && t[3] == '\0'){
            	return 1;
            } 
            else if((t[1] == '1' || t[1] == '2' ) && (t[2] >= '0' && t[2] <= '9') && t[3] == '\0'){
                return 1;
            }    
            else if ((t[1] >= '0' || t[1] <= '9') && t[2] == '\0') {  
                return 1;
            }
        }
    }
    return 0;
}

int chk_valid_g32_reg(char *t) {
    if (t[0] == 'W'||t[0]=='w') {
        if (t[1] >= '0' && t[1] <= '9') { 
            if (t[1] == '3' && (t[2] == '0' || t[2] == '1') && t[3] == '\0'){
            	return 1;
            } 
            else if((t[1] == '1' || t[1] == '2' ) && (t[2] >= '0' && t[2] <= '9') && t[3] == '\0'){
                return 1;
            }    
            else if ((t[1] >= '0' || t[1] <= '9') && t[2] == '\0') {  
                return 1;
            }
        }
    }
    return 0;
}

int isNumber(char *para)
{
	char *ptr = para;
	while (*ptr != '\0') {
        	if (isNum(*ptr)) {
                        ptr++;
                } else 
                	{
                        return 0;
                        }
                }
                return 1;

}
int chk_is_imme_div8(char *imme) {
    if (isHash(*imme)) {
        imme = imme + 1;  
        if (isZero(*imme)) {
            imme = imme + 1;  
        }
        if (isCross(*imme)) {
            imme = imme + 1;
            int number = strtol(imme, NULL, 16); 
            if (number % 8 == 0) {
                return 1; 
            }
        }
    }
    return 0; 
}
int chk_is_imme(char *imme) {
    if (isHash(*imme)) { 
        imme++; 
        if (*imme == '\0') {
            return 0; 
        }

        while (*imme != '\0') {
            if (!isNum(*imme)) {
                return 0; 
            }
            imme++;  
        }

        return 1;  
    }

    return 0;  
}

int is_valid_label(char *nw) {

    if (isColon(*(nw))) {
            char *ptr = nw +1;
            if (isAlpha(*ptr)) {
                ptr++;
                while (*ptr != '\0') {
                    if (isAlphaNum(*ptr)) {
                        ptr++;
                    } else {
                        break;
                    }
                }
                return 1;
            }
        }
        return 0;
}


Wordtype setcurrentwordtype(struct TA1state *t) {
     char* word = t->nw;
     char f=t->nw[0];
    if (strcmp(word, "mem") == 0) return MEM;
    else if (openparen(word[0])) return Openparan;
    else if (closeparen(word[0])) return Closeparan;
    else if (strcmp(word, "B") == 0) return BR;
    else if (isArrow(word)) return Arw;
    else if (isNewline(f)) return Newline;
    else if (isNumber(word)) return Parameter;
    else if (strcmp(word, "+<-") == 0) return PArw;
    else if (strcmp(word, "-<-") == 0) return SArw;
    else if (strcmp(word, "*<-") == 0) return MArw;
    else if (strcmp(word, "<-") == 0) return Arw;
    else if (word[0] == '+') { return Plus;}
    else if (word[0] == '-') return Minus;
    else if (word[0] == '*') return Mul;
     else if (strcmp(word, "<<") == 0) return LS;
     else if (strcmp(word, ">>") == 0) return RS;
    else if(is_valid_sbrname(word)==1) return Sbrname;
    else if(chk_valid_g64_reg(word)) return R64;
     else if(chk_valid_g32_reg(word)) return R32;
      else if(chk_is_imme_div8(word)) return Imm8; 
      else if(chk_is_imme(word)) return Imm;
    else if(is_valid_label(word)) return LABEL;
    else if (strcmp(word, "<=") == 0) return LEQ;
    else if (strcmp(word, ">=") == 0) return GEQ;
    else if (strcmp(word, "<") == 0) return LT;
    else if (strcmp(word, ">") == 0) return GT;
    else if (strcmp(word, "==") == 0) return DEQ;
    else if (strcmp(word, "!=") == 0) return NEQ;
    else if (strcmp(word, "CB") == 0) return CMPBranch;
    else if (strcmp(word, "Return") == 0) return RET;
    return Unknown;
}
Bool check(struct TA1state *t, Wordtype w) {
    Wordtype currentWordType = setcurrentwordtype(t);
    return currentWordType == w ? True : False;
}
void setinstructiontype(struct TA1state *t, Bool value)
{
        if(value==True)
        {
                t->instructionType=R32;
        }else
        {
                t->instructionType=R64;
        }

}
Wordtype getCurinstructiontype(struct TA1state *t) {
    return t->instructionType;
}
Wordtype getCurwordtype(struct TA1state *t)
{
    return setcurrentwordtype(t);
}
void validate(struct TA1state *t, Wordtype w) {
   
   Wordtype curwordtype = getCurwordtype(t);
    if (curwordtype != w) {
        exit(1);
    } 
}
Bool verify_instruction(TA1state t)
{
	Nextword(t);
	 instruction inst = {0};
	if(check(t,R32) || check(t,R64))
	{       
	        strcpy(inst.oprator[0], t->nw);
                inst.regCount+=1;
		setinstructiontype(t,getCurwordtype(t)==R32);
		Nextword(t);
		if(check(t,Arw))
		{   
		    Nextword(t);
		   
		    if(check(t,Imm))
		    {            strcpy(inst.oprator[1], t->nw);
                                 inst.regCount+=1;
		     	        Nextword(t);
				if (check(t,Newline)) 
                                {       inst.instype=mov;
                                        char *sbrname = t->sbrName;
                     			addInsToSbr(sbrname,&inst);
                     			verify_instruction(t);	 
                     	        }	
		    }
		    else if(check(t,MEM))
		    {
		    	   Nextword(t);
		    	   if (check(t,R32) || check(t,R64))
		    	   {       strcpy(inst.oprator[1], t->nw);
		    	           inst.regCount+=1;
		    		   Nextword(t);
		    		   if(t,Imm8)
		    		   {     
		    		        strcpy(inst.oprator[2], t->nw);
                                        inst.regCount+=1;
		    		        Nextword(t);
		    			if(check(t,Newline))
			                 {  
			                      inst.instype=load;
			                      char *sbrname = t->sbrName;
                     			      addInsToSbr(sbrname,&inst);
				              verify_instruction(t);
			                 }	
		    		    }
		    	    }
		    }
		    else 
		    {  	
			validate(t,getCurinstructiontype(t)==R32 ? R32:R64);
			setinstructiontype(t,getCurwordtype(t)==R32);
			strcpy(inst.oprator[1], t->nw);
		    	inst.regCount+=1;
		    	 Nextword(t);
 			if(check(t,Newline))
			{  
			        inst.instype=mov;
			        char *sbrname = t->sbrName;
                     	        addInsToSbr(sbrname,&inst);
				verify_instruction(t);
			}
			else if(check(t,Plus) ||check(t,Minus) ||check(t,Mul) ||check(t,LS) || check(t,RS))
			        
			{        
			         if(check(t,Plus)){inst.instype=add;}
			         else if(check(t,Minus)) { inst.instype=sub;}
			         else if(check(t,Mul)){ inst.instype=mul;}
			         else if(check(t,LS)){ inst.instype=Lshift;}
			         else if(check(t,RS)){ inst.instype=Rshift;}
				Nextword(t);
				if(check(t,Imm))
				{      strcpy(inst.oprator[2], t->nw);
                                       inst.regCount+=1;
					Nextword(t);
					if (check(t,Newline)) 
                                	{       
                                	      char *sbrname = t->sbrName;
                     			      addInsToSbr(sbrname,&inst);
                     				verify_instruction(t);	 
                     	        	}
				}else 
				{       	
				        
					validate(t,getCurinstructiontype(t)==R32 ? R32:R64);
					strcpy(inst.oprator[2], t->nw);
		    	                inst.regCount+=1;
					Nextword(t);
					if (check(t,Newline)) 
                                	{ 
                                	        char *sbrname = t->sbrName;
                     			      addInsToSbr(sbrname,&inst);
                     				verify_instruction(t);	 
                     	        	}
                     	        }
			} 
		    }
	 	}
		else if(check(t,PArw) || check(t,SArw) || check(t,MArw))
		{      
		        if(check(t,PArw)){inst.instype=add;}
			else if(check(t,SArw)) { inst.instype=sub;}
			else if(check(t,MArw)){ inst.instype=mul;}
			Nextword(t);
			if(check(t,Imm))
			{       
			          strcpy(inst.oprator[1], t->nw);
                                   inst.regCount+=1;
                                   Nextword(t);
				if (check(t,Newline)) 
                                {       
                                        char *sbrname = t->sbrName;
                     			 addInsToSbr(sbrname,&inst);
                     			verify_instruction(t);	 
                     	        }
			}
			else 
			{
				validate(t,getCurinstructiontype(t)==R32 ? R32:R64);
				strcpy(inst.oprator[1], t->nw);
		    	        inst.regCount+=1;
		    	        Nextword(t);
				if (check(t,Newline)) 
                                { 
                                        char *sbrname = t->sbrName;
                     			 addInsToSbr(sbrname,&inst);
                     			verify_instruction(t);	 
                     	        }
                     	}		
		}
	}	
	else if(check(t,MEM))
	{       
		Nextword(t);
		if(check(t,R64))
		{   
		        strcpy(inst.oprator[0], t->nw);
		    	inst.regCount+=1;
			Nextword(t);
			if(check(t,Imm8))
			{
			        strcpy(inst.oprator[1], t->nw);
                                inst.regCount+=1;
				Nextword(t);
				if(check(t,Arw))
			        {
					Nextword(t);
					if(check(t,R32) ||check(t,R64))
			                 {      strcpy(inst.oprator[2], t->nw);
		    	                         inst.regCount+=1;
				         	Nextword(t);
				         	if (check(t,Newline)) 
                                                {        inst.instype=str;
                                                         char *sbrname = t->sbrName;
                     			                addInsToSbr(sbrname,&inst);
                     			         	verify_instruction(t);
                     	                        }
				         }
				}
					
			}
		}
				
	}
	else if(check(t,BR))
	{       
         	Nextword(t);
                if (check(t,LABEL))
                {        inst.instype=branch;
                         strcpy(inst.oprator[0], t->nw);
		    	 inst.regCount+=1;
                         char *labelname = t->nw;
                         char *sbrname = t->sbrName;
                        addLabelToSbr(sbrname, labelname, 0,1);
                	Nextword(t);
			if (check(t,Newline)) 
                        {         
		    	          char *sbrname = t->sbrName;
                     	          addInsToSbr(sbrname,&inst);
                     		verify_instruction(t);	 
                        }
                }
                else if(check(t,Sbrname))
                {        inst.instype=branch_link;
                         strcpy(inst.oprator[0], t->nw);
		    	 inst.regCount+=1;
                        char *name = t->nw;
                        addSubroutine(name, 0, 1);
                	Nextword(t);
		  	if (check(t,Newline)) 
                        {       
		    	          char *sbrname = t->sbrName;
                     	          addInsToSbr(sbrname,&inst);
                     		verify_instruction(t);	 
                     	}
                } 	
	}
	else if(check(t,LABEL))
	{       strcpy(t->labelName, t->nw);
	        inst.instype=label;
	        strcpy(inst.oprator[0], t->nw);
		inst.regCount+=1;
	 	Nextword(t);
		  if (check(t,Newline)) 
                     {   
		    	          char *sbrname = t->sbrName;
                     	          addInsToSbr(sbrname,&inst);
	                	  char *labelname = t->labelName;
                        	  addLabelToSbr(sbrname, labelname, 1,0);
                     		  verify_instruction(t);	 
                     } 
                   
	}else if(check(t,RET))
	{
	 	Nextword(t);
	 	if(check(t,Newline))
	 	{       inst.instype=ret;
	 		char *sbrname = t->sbrName;
                     	addInsToSbr(sbrname,&inst);
	 		verify_instruction(t);
	 	}
	}
        else if(check(t,CMPBranch))
        {
        	Nextword(t);
        	if(check(t,R32) ||check(t,R64))
        	{       strcpy(inst.oprator[0], t->nw);
		    	 inst.regCount+=1;
        		setinstructiontype(t,getCurwordtype(t)==R32);
        		Nextword(t);
        		if(check(t,GEQ)||check(t,LEQ)||check(t,DEQ)||check(t,GT)||check(t,LT)||check(t,NEQ))
        		{       if(check(t,GEQ)){inst.instype=CMPB_GEQ;}
			         else if(check(t,LEQ)){inst.instype=CMPB_LEQ;}
			         else if(check(t,LT)){inst.instype=CMPB_LT;}
			         else if(check(t,GT)){inst.instype=CMPB_GT;}
			         else if(check(t,DEQ)){inst.instype=CMPB_DEQ;}
			         else if(check(t,NEQ)){inst.instype=CMPB_NEQ;}
        			Nextword(t);
        			validate(t,getCurinstructiontype(t)==R32 ? R32:R64);
        			strcpy(inst.oprator[1], t->nw);
		    	         inst.regCount+=1;
                                Nextword(t);
                                if(check(t,LABEL))
                                {	
                                	strcpy(inst.oprator[2], t->nw);
		    	                inst.regCount+=1;
                                	Nextword(t);
	 	                        if(check(t,Newline))
	 	                        {       
	 	                        	char *sbrname = t->sbrName;
                     	                        addInsToSbr(sbrname,&inst);
	 		                 	verify_instruction(t);
	 	                        }	
                                }
        		}
        	}	
        }
   
}

Bool verify_sbr_def(TA1state t)
{
    Nextword(t);
    if (check(t,Openparan))
    {
        Nextword(t);
        if (check(t,Sbrname))

        { strcpy(t->sbrName, t->nw);
          char *name = t->nw;
            addSubroutine(name, 1, 0); 
            Nextword(t);
            if (check(t,Parameter))
            {
                   Nextword(t);
                   if (check(t,Newline))
                   {
                      verify_instruction(t);
                      
                        if (check(t,Closeparan))
                        { 
                         Nextword(t);
                         verify_sbr_def(t);
                                return True;
                        }

                   }
            }
        }

     }
    return False;

}
                                                                                                                                              
Bool verify_program(TA1state t)
{
    Nextword(t);

    if (check(t,Openparan))
    {  
        Nextword(t);
        if (check(t,Sbrname))
        {   
            char *name = t->nw;
            strcpy(t->sbrName, t->nw);
            addSubroutine(t->sbrName, 0, 1); 
            Nextword(t);
            
           if (check(t,Newline))
            {

               verify_sbr_def(t);
                if (check(t,Closeparan))
                    { printf("validation status=%d\n",validateSubroutines());
                      	if(validateSubroutines())
                      	{ 	
                      		printf("verification status=%d\n",validateLabels());}
                                return True;
                        
                        
                      	}
             	     }
          
             }

     	}
    return False;
}

void printSubroutine(Subroutine s)
 {
    	printf("Name: %s\n", s.name);
    	printf("isDefine: %d\n", s.isDefine);
    	printf("isCall: %d\n", s.isCall);

        printf("Labels:\n");
    	for (int i = 0; i < s.labelCount; i++)
     	{
    		printf("  %s (isDefine: %d, isCall: %d)\n", s.labels[i].name,
               s.labels[i].isDefine, s.labels[i].isCall);
         }

    	printf("Label Count: %d\n\n", s.labelCount);

  	printf("Instructions:\n");
    	for (int i = 0; i < s.inscount; i++)
    	 {
       	 	instruction inst = s.inst[i]; 
       	 	printf("[");
        	printf("Instruction  Type: %d \n oprand count: %d \n", inst.instype, inst.regCount);
        	printf("Registers:\n");
        	
        	for (int j = 0; j < inst.regCount; j++) 
        	{
            		printf("  oprand[%d]: %s\t", j, inst.oprator[j]);
      		}
        	printf("]\n");
    	}
}
                                                                                                                                  
                                                                                                                                    
int main()
 {
    struct TA1state t;
    const char *input = " ( $mymul \n ( $mymul 2  \n CB X0 > X1 :moreadd \n X1 +<- X0 \n x0 <- x1 - x0 \n x1 <- x1 - x0 \n x3 <- #0  \n x2 <- #0 \n :moreadd \n CB x0 == x1 :nomoreadd \n x2 +<- x0 \n x3 +<- #1 \n B :moreadd \n :nomoreadd \n Return \n ) \n ) "; 
    TA1statebuild(&t, input);
    printf("|| Success || \n");
    if(verify_program(&t))
    {
    	generateARMCode("arm_prog.txt");
    }
     for (int i = 0; i < sbrCount; i++) {
        printf("Subroutine %d:\n", i + 1);
        printSubroutine(sbrarray[i]);
    }
 }