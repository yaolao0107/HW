#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define status int
#define ElemType int
#define TRUE 1
#define FALSE 0
typedef struct LNode
{
    ElemType data;
    struct LNode *nextL;
} LNode;
typedef struct CNode
{
    ElemType letter_num;
    struct CNode *nextC;
    LNode headnode;
} CNode;
typedef struct SNode
{
    ElemType clause_num;
    CNode bignode; //头结点
} SNode;

int var_num = 0, sen_num = 0;

status ReadFile(SNode *ps, char filename[100]);
status Traverse(SNode S);
status SaveCnfFile(SNode S, char filename[100]);
status Release_Link(SNode *ps);
ElemType select_letter(SNode *ps);
CNode *select_unitC(SNode *ps);
status Del_CwithL(SNode *ps, ElemType data);
status Del_LinC(SNode *ps, ElemType data);
status Empty_Clause(SNode *ps);
status DPLL(SNode *ps, int var, int flag);
status CopyList(SNode *ps1, SNode *ps2);
