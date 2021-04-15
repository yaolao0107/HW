#ifndef _HEAD_H
#define _HEAD_H
#define _CRT_SECURE_NO_WARNINGS
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
    struct LNode* nextL;
} LNode;
typedef struct CNode
{
    ElemType letternum;
    struct CNode* nextC;
    LNode headnode;
} CNode;
typedef struct SNode
{
    ElemType clausenum;
    CNode bignode;
} SNode;
extern int var_num, sen_num;
extern int* truthtable;
extern char target_filename[50];
extern status ReadFile(SNode* ps, char filename[100]);
extern status Traverse(SNode S);
extern status SaveCnfFile(SNode S);
extern status ReleaseLink(SNode* ps);
extern ElemType SelectLetter(SNode* ps);
extern ElemType SelectLetter_og(SNode* ps);
extern CNode* SelectUnitC(SNode* ps);
extern status Del_CwithL(SNode* ps, ElemType data);
extern status Del_LinC(SNode* ps, ElemType data);
extern status IfEmptyClause(SNode* ps);
extern status DPLL(SNode* ps, int var, int flag, int truthtable[]);
extern status CopyList(SNode* ps1, SNode* ps2);
extern status Assert(int var, int flag, int truthtable[]);
extern status TraverseTruthtable(int truthtable[]);
extern status SudoTransfer(int order);
extern void ChoosePuzzle(int truthtable[]);
extern void CheckClausenum(SNode* ps);
extern int UniqueCombine(int* combine, int* target, int* data, int target_num);
extern status ShowPuzzle(int sudo_table[6][6], int order);
extern status IfCompleted(int sudo_table[6][6], int order);
extern status SaveSolution(int flag, int truthtable[], clock_t cost);
extern status DPLL_og(SNode* ps, int var, int flag, int truthtable[]);

#endif
