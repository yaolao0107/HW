#include "head.h"
/*cnf文件读取函数*/
status ReadFile(SNode *ps, char filename[100])
{
    char ch;
    FILE *fp = NULL;
    CNode *pc, *pc_new;
    LNode *pl, *pl_new;
    int buffer = 0;
    if (((fp = fopen(filename, "r")) == NULL))
    {
        printf("Fail to open cnf File!\n");
        return FALSE;
    }
    else
    {
        while (fread(&ch, sizeof(char), 1, fp))
        {
            if (ch != 'p')
                printf("%c", ch);
            else
                break;
        }
    }
    for (int i = 0; i < 5; i++)
        fread(&ch, sizeof(char), 1, fp);
    fscanf(fp, "%d", &var_num);
    fscanf(fp, "%d", &sen_num);

    int flag_letter_1st = 1, flag_clause_1st = 1, flag_newC = 1;
    while (fscanf(fp, "%d", &buffer) != EOF)
    {
        //读取文字,读取值非0时，生成文字结点
        if (buffer)
        {
            pl_new = (LNode *)malloc(sizeof(LNode));
            pl_new->data = buffer;
            pl_new->nextL = NULL;
        }
        //读取值为0时生成下一子句
        else
        {
            flag_letter_1st = 1;
            flag_newC = 1;
            pl->nextL = NULL;
            continue;
        }
        //在生成第一个子句时，让S与C建立联系
        if (flag_newC && flag_clause_1st)
        {
            pc_new = (CNode *)malloc(sizeof(CNode));
            pc_new->headnode.nextL = NULL;
            pc_new->letternum = 0;
            pc_new->nextC = NULL;
            ps->bignode.nextC = pc_new;
            ps->clausenum = sen_num;
            pc = ps->bignode.nextC;
            flag_clause_1st = 0;
            flag_newC = 0;
        }
        //要生成非首子句
        else if (flag_newC && !flag_clause_1st)
        {
            pc_new = (CNode *)malloc(sizeof(CNode));
            pc_new->headnode.nextL = NULL;
            pc_new->letternum = 0;
            pc_new->nextC = NULL;
            pc->nextC = pc_new;
            pc = pc->nextC;
            flag_newC = 0;
        }
        //在生成子句的首文字时建立C与L的联系
        if (flag_letter_1st)
        {
            pc->headnode.nextL = pl_new;
            pc->letternum++;
            pl = pc->headnode.nextL;
            flag_letter_1st = 0;
        }
        //将生成的文字结点纳入字句，准备下一文字的生成
        else
        {
            pl->nextL = pl_new;
            pl = pl->nextL;
            pc->letternum++;
        }
    }
    CheckClausenum(ps);
    fclose(fp);
    return TRUE;
}

//cnf文件的公式解析
status Traverse(SNode S)
{
    CNode *pc = NULL;
    LNode *pl = NULL;
    if (!S.clausenum)
    {
        printf("子句集为空!\n");
        return FALSE;
    }
    pc = S.bignode.nextC;
    printf("__________字句内容如下_________\n");
    //双层循环读取子句集
    while (pc)
    {
        pl = pc->headnode.nextL;
        while (pl)
        {
            printf("%6d ", pl->data);
            pl = pl->nextL;
        }
        printf("\n");
        pc = pc->nextC;
    }
    return TRUE;
}
//cnf文件的保存
status SaveCnfFile(SNode S)
{

    FILE *fp = NULL;
    CNode *pc = NULL;
    LNode *pl = NULL;
    if (((fp = fopen("D:\\tst.txt", "wb")) == NULL))
    {
        printf("File open error!\n");
        return FALSE;
    }
    else
    {
        fprintf(fp, "%s", "p cnf ");
        fprintf(fp, "%d ", var_num);
        fprintf(fp, "%d \n", sen_num);
        pc = S.bignode.nextC;
        while (pc)
        {
            pl = pc->headnode.nextL;
            while (pl)
            {
                fprintf(fp, "%d ", pl->data);
                pl = pl->nextL;
            }
            fprintf(fp, "0 \n");
            pc = pc->nextC;
        }
        fclose(fp);
        return TRUE;
    }
}

//释放链表空间
status ReleaseLink(SNode *ps)
{
    CNode *pc;
    LNode *pl;
    while (ps->bignode.nextC)
    {
        pc = ps->bignode.nextC;
        while (pc->headnode.nextL)
        {
            pl = pc->headnode.nextL;
            pc->headnode.nextL = pc->headnode.nextL->nextL;
            free(pl);
        }
        ps->bignode.nextC = ps->bignode.nextC->nextC;
        free(pc);
    }
    ps->clausenum = 0;
    return TRUE;
}
//新增check函数，检查子句集的子句数是否正确
void CheckClausenum(SNode *ps)
{
    int count = 0;
    CNode *pc = NULL;
    pc = ps->bignode.nextC;
    //双层循环读取子句集
    while (pc)
    {
        count++;
        pc = pc->nextC;
    }
    ps->clausenum = count;
}status SaveSolution(int flag,int truthtable[],clock_t cost)
{
    FILE *fp=NULL;
    if((fp=fopen("D:\\solution.txt","w"))==NULL)
    {
        printf("File Open Erorr!\n");
        return FALSE;
    }
    fprintf(fp,"s %d\n",flag);
    if(flag)
    {
    fprintf(fp,"v ");
    for (int count = 0; count < var_num; count++)
    {
        if(truthtable[count]>0)
        fprintf(fp,"%d ",count+1);
        else
        fprintf(fp,"%d ",-(count+1));
    }
    }
    fprintf(fp,"\nt %ldms\n",cost);
    fclose(fp);
    return TRUE;
}
