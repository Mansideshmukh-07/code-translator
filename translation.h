#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char* removeFirstChar(char* str) {
    if (str == NULL || strlen(str) == 0) {
        return str; 
    }
    memmove(str, str + 1, strlen(str)); 
    return str;
}
void generateARMCode(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    fprintf(file, ".global _start\n");
    fprintf(file, "_start:\n");
     char tempName[MAX_SUB];
        strcpy(tempName, sbrarray[0].name); // Copy the name to temp
        removeFirstChar(tempName);
    fprintf(file, "    BL __SBR__%s\n",tempName);
    fprintf(file,"\n");
    for (int i = 0; i < sbrCount; i++) {
    removeFirstChar(sbrarray[i].name);
        fprintf(file, "__SBR__%s:\n", sbrarray[i].name);
        fprintf(file, "\n");
        fprintf(file,"    STR FP,#[SP ,#-8]!\n");
        fprintf(file,"    STR LR,#[SP ,#-8]!\n");
        fprintf(file,"    MOV FP,SP\n");
        fprintf(file,"\n");
        for (int j = 0; j < sbrarray[i].inscount; j++) {
            instruction inst = sbrarray[i].inst[j];
            switch (inst.instype) {
                case str: {
                    if (inst.regCount >= 2) {
                        fprintf(file, "    STR %s, [%s, %s]\n", inst.oprator[2], inst.oprator[0], inst.oprator[1]);
                    }
                    break;
                }
                case load: {
                    if (inst.regCount >= 2) {
                        fprintf(file, "    LDR %s, [%s, %s]\n", inst.oprator[0], inst.oprator[1], inst.oprator[2]);
                    }
                    break;
                }
                case branch: {
                    if (inst.regCount >= 1) {
                        removeFirstChar(inst.oprator[0]);
                        fprintf(file, "    B __%s__%s \n",sbrarray[i].name, inst.oprator[0]);
                                fprintf(file,"\n");
                    }
                    break;
                }
                case branch_link: {
                    if (inst.regCount >= 1) {
                        removeFirstChar(inst.oprator[0]);
                        fprintf(file, "    BL __SBR__%s \n",inst.oprator[0]);
                    }
                    break;
                }
                case ret: {
                       fprintf(file,"    B __%s__RET\n ",sbrarray[i].name);
                    break;
                }
                case label: {
                    if (inst.regCount >= 1) {
                        removeFirstChar(inst.oprator[0]);
                        fprintf(file, "    __%s__%s: \n",sbrarray[i].name, inst.oprator[0]);
                                fprintf(file,"\n");
                    }
                    break;
                }
                case Lshift: {
                    if (inst.regCount >= 3) {
                        fprintf(file, "    LSL %s, %s, %s \n",inst.oprator[0],inst.oprator[1],inst.oprator[2]);
                    }
                    break;
                }
                case Rshift: {
                    if (inst.regCount >= 3) {
                        fprintf(file, "    LSR %s, %s, %s \n",inst.oprator[0],inst.oprator[1],inst.oprator[2]);
                    }
                    break;
                }
                case CMPB_LT: {
                    if (inst.regCount >= 3) {
                        fprintf(file, "    CMP %s, %s \n",inst.oprator[0],inst.oprator[1]);
                        removeFirstChar(inst.oprator[2]);
                        fprintf(file, "    BLT __%s__%s \n",sbrarray[i].name,inst.oprator[2]);
                        fprintf(file,"\n");
                    }
                    break;
                }
                case CMPB_GT: {
                    if (inst.regCount >= 3) {
                        fprintf(file, "    CMP %s, %s \n",inst.oprator[0],inst.oprator[1]);
                        removeFirstChar(inst.oprator[2]);
                        fprintf(file, "    BGT __%s__ %s \n",sbrarray[i].name,inst.oprator[2]);
                        fprintf(file,"\n");
                    }
                    break;
                }
                case CMPB_GEQ: {
                    if (inst.regCount >= 3) {
                        fprintf(file, "    CMP %s, %s \n",inst.oprator[0],inst.oprator[1]);
                        removeFirstChar(inst.oprator[2]);
                        fprintf(file, "    BGE __%s__%s \n",sbrarray[i].name,inst.oprator[2]);
                        fprintf(file,"\n");
                    }
                    break;
                }
                case CMPB_LEQ: {
                    if (inst.regCount >= 3) {
                        fprintf(file, "    CMP %s, %s \n",inst.oprator[0],inst.oprator[1]);
                        removeFirstChar(inst.oprator[2]);
                        fprintf(file, "    BLE __%s__%s \n",sbrarray[i].name,inst.oprator[2]);
                        fprintf(file,"\n");
                    }
                    break;
                }
                case CMPB_DEQ: {
                    if (inst.regCount >= 3) {
                        fprintf(file, "    CMP %s, %s \n",inst.oprator[0],inst.oprator[1]);
                        removeFirstChar(inst.oprator[2]);
                        fprintf(file, "    BEQ __%s__%s \n",sbrarray[i].name,inst.oprator[2]);
                        fprintf(file,"\n");
                    }
                    break;
                }
                case CMPB_NEQ: {
                    if (inst.regCount >= 3) {
                        fprintf(file, "    CMP %s, %s \n",inst.oprator[0],inst.oprator[1]);
                        removeFirstChar(inst.oprator[2]);
                        fprintf(file, "    BNE __%s__%s \n",sbrarray[i].name,inst.oprator[2]);
                        fprintf(file,"\n");
                    }
                    break;
                }
                case add: {
                    
                    if (inst.regCount >= 3) {
                        fprintf(file, "    ADD %s, %s, %s\n", inst.oprator[0], inst.oprator[1], inst.oprator[2]);
                    }
                    else if(inst.regCount >=2)
                     {
                      	fprintf(file,"    ADD %s, %s, %s\n",inst.oprator[0],inst.oprator[0],inst.oprator[1]);
                     }
                    break;
                }
                case sub: {
                    
                    if (inst.regCount >= 3) {
                        fprintf(file, "    SUB %s, %s, %s\n", inst.oprator[0], inst.oprator[1], inst.oprator[2]);
                    }
                    else if(inst.regCount >=2)
                     {
                      	fprintf(file,"    SUB %s, %s, %s\n",inst.oprator[0],inst.oprator[0],inst.oprator[1]);
                     }
                    break;
                }
                case mul: {
                    
                    if (inst.regCount >= 3) {
                        fprintf(file, "    MUL %s, %s, %s\n", inst.oprator[0], inst.oprator[1], inst.oprator[2]);
                    }
                    else if(inst.regCount >=2)
                     {
                      	fprintf(file,"    MUL %s, %s, %s\n",inst.oprator[0],inst.oprator[0],inst.oprator[1]);
                     }
                    break;
                }

                case mov: {
                    if (inst.regCount >= 2) {
                        fprintf(file, "    MOV %s, %s\n", inst.oprator[0], inst.oprator[1]);
                    }
                    break;
                }
            }
        }
          fprintf(file,"\n");
          fprintf(file,"    __%s__RET:\n",sbrarray[i].name);
          fprintf(file,"\n");
          fprintf(file,"    MOV SP, FP\n");
          fprintf(file,"    LDR FP, [SP], #-8\n");
          fprintf(file,"    LDR LR, [SP], #-8\n");
          fprintf(file,"    RET\n");
          fprintf(file,"\n");
    }

    fclose(file);  
}