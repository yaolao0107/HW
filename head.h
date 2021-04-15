#ifndef _HEAD_H
#define _HEAD_H
/**
文件名:head.h
功能:定义数据结构和声明的函数原型
*/
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

typedef struct LNode//文字结点
{
    ElemType data;//文字内容
    struct LNode* nextL;//指向下一个文字的指针
} LNode;
typedef struct CNode//子句结点
{
    ElemType letternum;//子句长度
    struct CNode* nextC;//指向下一个子句的指针
    LNode headnode;//子句的文字头结点
} CNode;
typedef struct SNode//子句集结点
{
    ElemType clausenum;//子句集的长度
    CNode bignode;//子句集的子句头结点
} SNode;
extern int var_num, sen_num;//子句集的变元个数和子句个数
extern int* truthtable;//真值表
extern char target_filename[50];//数独的读取路径
//cnfPraser部分的函数声明
extern status ReadFile(SNode* ps, char filename[100]);
extern status Traverse(SNode S);
extern status SaveCnfFile(SNode S);
extern status ReleaseLink(SNode* ps);
extern void CheckClausenum(SNode* ps);

//DPLL部分的函数声明
extern ElemType SelectLetter(SNode* ps);
extern ElemType SelectLetter_og(SNode* ps);
extern CNode* SelectUnitC(SNode* ps);
extern status Del_CwithL(SNode* ps, ElemType data);
extern status Del_LinC(SNode* ps, ElemType data);
extern status IfEmptyClause(SNode* ps);
extern status DPLL(SNode* ps, int var, int flag, int truthtable[]);
extern status DPLL_og(SNode* ps, int var, int flag, int truthtable[]);
extern status CopyList(SNode* ps1, SNode* ps2);
extern status Assert(int var, int flag, int truthtable[]);
extern status TraverseTruthtable(int truthtable[]);

//BinarySudo部分的函数声明
extern status SudoTransfer(int order);
extern void ChoosePuzzle(int truthtable[]);
extern int UniqueCombine(int* combine, int* target, int* data, int target_num);
extern status ShowPuzzle(int sudo_table[6][6], int order);
extern status IfCompleted(int sudo_table[6][6], int order);
extern status SaveSolution(int flag, int truthtable[], clock_t cost);

#endif
