#include "head.h"
/*输入子句集S，按照MOM策略选择子句集中一个文字，并且输出文字的data值*/
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

ElemType SelectLetter_og(SNode* ps)
{
    ElemType var = rand() % var_num + 1;
    return var;
}
//选择一个单子句,选中后返回指向单子句的指针pct，否则返回NULL
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

//删除含文字L的所有子句
//ps为子句集，data为要删除的文字
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

//删除所有子句中的文字L
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
//子句集S中含有空子句，返回1，否则返回0
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
//给出子句集S，若可满足，输出TRUE,否则输出FALSE
//var非0时激活分支策略,flag为var的真值情况
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
//将ps1所指的子句集复制给ps2
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
//新增赋值函数，文字var的真假值为flag，将其保存到真值表truthtable中去
status Assert(int var, int flag, int truthtable[])
{
    if (!var)
        return FALSE;
    else if (var > 0)
        truthtable[var - 1] = flag;
    else
        truthtable[abs(var) - 1] = !flag;
}
//新增打印函数，打印真值表
status TraverseTruthtable(int truthtable[])
{
    printf("---------------输出真值表的内容--------------\n\n");
    printf("注:1->真    2->假   -1->真或者假\n\n");
    for (int i = 0; i < var_num; i++)
        printf("文字%4d:%4d\n", i + 1, truthtable[i]);
    return TRUE;
}
