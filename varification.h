#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SUB 50
#define MAX_LABELS 100
#define MAX_INSTRUCTIONS 10000
#define MAX_LABEL_NAME_LENGTH 255
typedef enum {
    add, sub, mul, load, str, branch,branch_link, ret,label,mov,Lshift,Rshift,
    CMPB_LEQ,CMPB_GEQ,CMPB_LT,CMPB_GT,CMPB_NEQ,CMPB_DEQ
} instructiontype;
typedef struct {
    instructiontype instype;                       
    char oprator[3][10];        
    int regCount;             
} instruction;

typedef struct {
    char name[MAX_SUB];
    int isDefine;
    int isCall;
} Label;

typedef struct {
    char name[MAX_SUB];
    int isDefine;
    int isCall;
    Label labels[MAX_LABELS];
    int labelCount;
    instruction inst[MAX_INSTRUCTIONS];
    int inscount;
} Subroutine;
Subroutine sbrarray[MAX_SUB];
int sbrCount = 0;

int findSubroutine(char *name)
 {
    for (int i = 0; i < sbrCount; i++)
     {
        if (strcmp(sbrarray[i].name, name) == 0)
        {
            return i;
        }
     }
    return -1;
}

int findLabelInSbr(char *sbrName, char *labelName) 
{
    int sbrIndex = findSubroutine(sbrName);
   
    if (sbrIndex != -1)
     {
        for (int j = 0; j < sbrarray[sbrIndex].labelCount; j++)
         {
            if (strcmp(sbrarray[sbrIndex].labels[j].name, labelName) == 0)
            {
                return j;
            }
         }
     }
     return -1;
}


void addSubroutine(char *name, int isDefined, int isCalled)
 {
    int index = findSubroutine(name);
     
    if (index == -1)
     {
        if (sbrCount < MAX_SUB)
         {
            strcpy(sbrarray[sbrCount].name, name);
            sbrarray[sbrCount].isDefine = isDefined;
            sbrarray[sbrCount].isCall = isCalled;
            sbrCount++;
         }
     } else 
     {
        if (isCalled) sbrarray[index].isCall = 1;
        if (isDefined && sbrarray[index].isDefine == 0) sbrarray[index].isDefine = 1;
     }
}

void addLabelToSbr(char *sbrName, char *labelName, int isDefine, int isCall)
 {
    int sbrIndex = findSubroutine(sbrName);
 
    if (sbrIndex == -1)
     {
        return;
     }

    int labelIndex = findLabelInSbr(sbrName, labelName);
    if (labelIndex == -1)
     {
        if (sbrarray[sbrIndex].labelCount < MAX_LABELS)
         {
            strcpy(sbrarray[sbrIndex].labels[sbrarray[sbrIndex].labelCount].name, labelName);
            sbrarray[sbrIndex].labels[sbrarray[sbrIndex].labelCount].isDefine = isDefine;
            sbrarray[sbrIndex].labels[sbrarray[sbrIndex].labelCount].isCall = isCall;
            sbrarray[sbrIndex].labelCount++;
         }
     } else
      {
        if (isCall)
         {
        
            sbrarray[sbrIndex].labels[labelIndex].isCall = 1;
         }
        if (isDefine && sbrarray[sbrIndex].labels[labelIndex].isDefine == 0)
         {
            sbrarray[sbrIndex].labels[labelIndex].isDefine = 1; 
             
         }
    }
}

int validateLabels()
 {
    for (int i = 0; i < sbrCount; i++)
     { 
        for (int j = 0; j < sbrarray[i].labelCount; j++)
        {
            if (sbrarray[i].labels[j].isCall == 1 && sbrarray[i].labels[j].isDefine == 0)
            {
                return 0; 
            }
        }
    }
    return 1; 
}

int validateSubroutines()
 {
    for (int i = 0; i < sbrCount; i++)
     {
        if (sbrarray[i].isDefine == 0 && sbrarray[i].isCall == 1)
        {
            return 0;
        }
    }
    return 1;
}

void addInsToSbr(char *sbrname, instruction *inst)
 {

    int sbrIndex = findSubroutine(sbrname);
    
    if (sbrIndex == -1)
    {
        printf("Subroutine '%s' not found.\n", sbrname);
        return;
    }
   
    if (sbrarray[sbrIndex].inscount < MAX_INSTRUCTIONS)
     {
    	sbrarray[sbrIndex].inst[sbrarray[sbrIndex].inscount] = *inst;
   	 sbrarray[sbrIndex].inscount++;
     }
}