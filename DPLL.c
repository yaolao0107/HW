#include "head.h"
/**
文件名:DPLL
功能:SAT求解器
*/

/*************************************************************
函数名称:SelectLetter
函数功能:分支策略中基于MOM选择一个特殊的变元
输入的参数:指向子句集结点的指针ps
返回值:最短子句中的一个文字
**************************************************************/
ElemType SelectLetter(SNode* ps)
{
    int min = ps->bignode.nextC->letternum;
    CNode* pc = ps->bignode.nextC;
    CNode* pct = ps->bignode.nextC; //t=traverse
    while (pct)
    {
        if (min <= pct->letternum)
        {
            pct = pct->nextC;
            continue;
        }
        min = pct->letternum;
        pc = pct;
        pct = pct->nextC;
    }                                //寻找最短子句（一个）
    return pc->headnode.nextL->data; //选最短子句的一个文字
}
/*************************************************************
函数名称:SelectLetter_og
函数功能:分支策略中基于随机选择选择一个特殊的变元
输入的参数:指向子句集结点的指针ps
返回值:某一个1-var_num之间的文字
**************************************************************/
ElemType SelectLetter_og(SNode* ps)
{
    ElemType var = rand() % var_num + 1;
    return var;
}
/*************************************************************
函数名称:SelectUnitC
函数功能:查找并返回一个子句集中的单子句
输入的参数:指向子句集结点的指针ps
返回值:若找到则返回单子句的地址，否则返回NULL
**************************************************************/
CNode* SelectUnitC(SNode* ps)
{
    CNode* pc = ps->bignode.nextC;
    while (pc)
    {
        if (pc->letternum == 1)
            return pc;
        else
            pc = pc->nextC;
    }
    return NULL;
}

/*************************************************************
函数名称:Del_CwithL
函数功能:删除所有含有文字L的子句C
输入的参数:指向子句集结点的指针ps，文字data
返回值:无意义
**************************************************************/
status Del_CwithL(SNode* ps, ElemType data)
{
    CNode* pc = &ps->bignode, * qc = ps->bignode.nextC; //pc指向子句集的头节点，qc指向首子句
    LNode* pl = qc->headnode.nextL;                    //pl指向qc指向子句的首文字
    int count = 0;
    int error = 0;                                     //测试bug部分计数器
    while (qc)
    {
        int flag = 0; //flag为是否删除当前子句qc的标志,每次遍历置0
        pl = qc->headnode.nextL;
        while (pl)
        {
            if (pl->data == 10141)
                error = 1;
            if (pl->data == data)
            {
                flag = 1;
                break;
            }
            pl = pl->nextL;
        }
        if (flag == 1)
        {
            pc->nextC = qc->nextC;
            free(qc);
            qc = pc->nextC;
            ps->clausenum--;
            count++;
            continue;//最终章
        }
        pc = pc->nextC;
        if (qc)
            qc = qc->nextC;
    }
    return TRUE;
}

/*************************************************************
函数名称:Del_LinC
函数功能:删除所有子句中的文字L
输入的参数:指向子句集结点的指针ps，文字data
返回值:无意义
**************************************************************/
status Del_LinC(SNode* ps, ElemType data)
{
    CNode* pc = ps->bignode.nextC;
    LNode* pl;
    LNode* ql;
    while (pc)
    {
        pl = &pc->headnode;
        ql = pl->nextL;
        while (ql)
        {
            if (ql->data == data)
            {
                pl->nextL = ql->nextL;
                free(ql);
                ql = pl->nextL;
                pc->letternum--;
            }
            pl = pl->nextL;
            if (ql)
                ql = ql->nextL;
        }
        pc = pc->nextC;
    }
    return TRUE;
}
/*************************************************************
函数名称:IfEmptyClause
函数功能:判断子句集中是否含有空子句
输入的参数:指向子句集结点的指针ps
返回值:若有空子句，返回TRUE，否则返回FALSE
**************************************************************/
status IfEmptyClause(SNode* ps)
{
    CNode* pc = ps->bignode.nextC;
    while (pc)
    {
        if (!pc->letternum)
            return TRUE;
        else
            pc = pc->nextC;
    }
    return FALSE;
}
/*************************************************************
函数名称:DPLL
函数功能:SAT问题的求解器
输入的参数:指向子句集结点的指针ps，分支策略选择的文字var和值，真值表truthtable
返回值:若SAT可满足，返回TRUE，否则返回FALSE
**************************************************************/
status DPLL(SNode* ps, int var, int flag, int truthtable[])
{
    //分治策略,var非0激活分治策略
    if (var && flag)
    {
        Del_CwithL(ps, var);
        Del_LinC(ps, -var);
    }
    else if (var && !flag)
    {
        Del_CwithL(ps, -var);
        Del_LinC(ps, var);
    }
    CNode* pct = NULL;
    ElemType data = 0; //保存单子句中的文字
    //单子句策略
    while (pct = SelectUnitC(ps))
    {
        data = pct->headnode.nextL->data;
        Assert(data, 1, truthtable);
        Del_CwithL(ps, data);
        Del_LinC(ps, -data);
        if (!ps->clausenum)
            return TRUE;
        else if (IfEmptyClause(ps))
            return FALSE;
    }
    ElemType v = SelectLetter(ps); //文字取真
    SNode* ps_former = (SNode*)malloc(sizeof(SNode));
    CopyList(ps, ps_former);
    if (DPLL(ps, v, 0, truthtable))
    {
        Assert(v, 0, truthtable);
        return TRUE;
    }
    else if (DPLL(ps_former, v, 1, truthtable))
    {
        Assert(v, 1, truthtable);
        return TRUE;
    }
    else
        return FALSE;
}
/*************************************************************
函数名称:DPLL_og
函数功能:SAT问题的求解器(使用select_og分支策略)
输入的参数:指向子句集结点的指针ps，分支策略选择的文字var和值，真值表truthtable
返回值:若SAT可满足，返回TRUE，否则返回FALSE
**************************************************************/
status DPLL_og(SNode* ps, int var, int flag, int truthtable[])
{
    //分治策略,var非0激活分治策略
    if (var && flag)
    {
        Del_CwithL(ps, var);
        Del_LinC(ps, -var);
    }
    else if (var && !flag)
    {
        Del_CwithL(ps, -var);
        Del_LinC(ps, var);
    }
    CNode* pct = NULL;
    ElemType data = 0; //保存单子句中的文字
    //单子句策略
    while (pct = SelectUnitC(ps))
    {
        data = pct->headnode.nextL->data;
        Assert(data, 1, truthtable);
        Del_CwithL(ps, data);
        Del_LinC(ps, -data);
        if (!ps->clausenum)
            return TRUE;
        else if (IfEmptyClause(ps))
            return FALSE;
    }
    ElemType v = SelectLetter_og(ps); //文字取真
    SNode* ps_former = (SNode*)malloc(sizeof(SNode));
    CopyList(ps, ps_former);
    if (DPLL_og(ps, v, 0, truthtable))
    {
        Assert(v, 0, truthtable);
        return TRUE;
    }
    else if (DPLL_og(ps_former, v, 1, truthtable))
    {
        Assert(v, 1, truthtable);
        return TRUE;
    }
    else
        return FALSE;
}
/*************************************************************
函数名称:CopyList
函数功能:复制一个子句集
输入的参数:指向子句集结点的指针ps1，和待生成子句集指针ps2
返回值:无意义
**************************************************************/
status CopyList(SNode* ps1, SNode* ps2)
{
    CNode* pc1 = NULL, * pc2 = NULL;
    LNode* pl1 = NULL, * pl2 = NULL;
    ps2->clausenum = ps1->clausenum;
    pc1 = &ps1->bignode;
    pc2 = &ps2->bignode;
    while (pc1->nextC)
    {
        pc1 = pc1->nextC;
        pc2->nextC = (CNode*)malloc(sizeof(CNode));
        pc2 = pc2->nextC;
        pc2->nextC = NULL;
        pc2->letternum = pc1->letternum;

        pl1 = &pc1->headnode;
        pl2 = &pc2->headnode;
        while (pl1->nextL)
        {
            pl1 = pl1->nextL;
            pl2->nextL = (LNode*)malloc(sizeof(LNode));
            pl2 = pl2->nextL;
            pl2->data = pl1->data;
            pl2->nextL = NULL;
        }
    }
    return 0;
}
/*************************************************************
函数名称:Assert
函数功能:对文字进行赋值
输入的参数文字var和值flag，真值表truthtable
返回值:若文字为0,则返回FALSE
**************************************************************/
status Assert(int var, int flag, int truthtable[])
{
    if (!var)
        return FALSE;
    else if (var > 0)
        truthtable[var - 1] = flag;
    else
        truthtable[abs(var) - 1] = !flag;
}
/*************************************************************
函数名称:TraverseTruthtable
函数功能:SAT问题求解结果的输出
输入的参数:真值表truthtable
返回值:无意义
**************************************************************/
status TraverseTruthtable(int truthtable[])
{
    printf("---------------输出真值表的内容--------------\n\n");
    printf("注:1->真    2->假   -1->真或者假\n\n");
    for (int i = 0; i < var_num; i++)
        printf("文字%4d:%4d\n", i + 1, truthtable[i]);
    return TRUE;
}
/*************************************************************
函数名称:SaveSolution
函数功能:保存SAT问题的解
输入的参数:求解成功的标志flag，真值表truthtable和时间开销cost
返回值:无意义
**************************************************************/
status SaveSolution(int flag, int truthtable[], clock_t cost)
{
    FILE* fp = NULL;
    if ((fp = fopen("solution.txt", "w")) == NULL)
    {
        printf("File Open Erorr!\n");
        return FALSE;
    }
    fprintf(fp, "s %d\n", flag);
    if (flag)
    {
        fprintf(fp, "v ");
        for (int count = 0; count < var_num; count++)
        {
            if (truthtable[count] > 0)
                fprintf(fp, "%d ", count + 1);
            else
                fprintf(fp, "%d ", -(count + 1));
        }
    }
    fprintf(fp, "\nt %ldms\n", cost);
    fclose(fp);
    return TRUE;
}
