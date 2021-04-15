#include"head.h"
int var_num = 0, sen_num = 0;
char target_filename[50];
int* truthtable = NULL;
SNode S;
int main()
{
    FILE* fp = NULL;
    char filename[100], filename_copy[50];
    int op = 1, op_sat = 1, op_sudo = 1;
    int row, col;
    int in_or_not = 1;
    int truth, flag;

    clock_t start_t, end_t;
    clock_t cost;
    S.clausenum = 0;
    S.bignode.headnode.nextL = NULL;
    S.bignode.headnode.data = 0;
    S.bignode.letternum = 0;
    S.bignode.nextC = NULL;
    int order = 0;
    while (op)
    {
        while (op)
        {
            system("cls");
            printf("\n               Menu for SuDoku or SAT              \n");
            printf("------------------------------------------------------\n");
            printf("            1.SAT                  2.SuDoKu           \n");
            printf("            0.Exit                                    \n");
            printf("------------------------------------------------------\n");
            if (scanf("%d", &op))
            {
                op_sat = op_sudo = 1;
                switch (op)
                {
                case 1:
                    while (op_sat)
                    {
                        system("cls");
                        printf("\n              Menu for SAT                \n");
                        printf("-------------------------------------------\n");
                        printf("     1.ReadCnfFile           2.TraverseCnf  \n");
                        printf("     3.SaveCnfFile           4.DPLL        \n");
                        printf("     5.SaveSolution          6.DPLL Compare \n");
                        printf("     0.Exit                                   \n");
                        printf("--------------------------------------------\n");
                        if (scanf("%d", &op_sat))
                        {
                            switch (op_sat)
                            {
                            case 1:
                                printf("input filename:");
                                scanf("%s", filename);
                                ReleaseLink(&S);
                                in_or_not = 0;
                                if (ReadFile(&S, filename) == TRUE)
                                {
                                    printf("cnf文件打开成功!\n");
                                    in_or_not = 1;
                                }
                                else
                                    printf("cnf文件打开失败!\n");
                                getchar();
                                getchar();
                                break;
                            case 2:
                                if (in_or_not == 1)
                                {
                                    Traverse(S);
                                    printf("cnf遍历成功!\n");
                                }
                                else
                                    printf("cnf遍历失败!\n");
                                getchar();
                                getchar();
                                break;
                            case 3:
                                Del_CwithL(&S, -6);
                                Del_LinC(&S, 6);
                                if (SaveCnfFile(S) == TRUE)
                                    printf("cnf保存成功!\n");
                                else
                                    printf("cnf保存失败!\n");
                                getchar();
                                getchar();
                                break;
                            case 4:
                                printf("即将启动DPLL算法解决SAT问题\n");
                                start_t = clock();
                                truthtable = (int*)malloc(var_num * sizeof(int));
                                for (int i = 0; i < var_num; i++)
                                {
                                    truthtable[i] = -1; //初始化真值表
                                }
                                if (flag = DPLL(&S, 0, 0, truthtable))
                                {
                                    printf("SAT有解且解如下所示\n");
                                    TraverseTruthtable(truthtable);
                                }
                                else
                                    printf("SAT无解\n");
                                end_t = clock();
                                cost = 1000 * (end_t - start_t) / CLOCKS_PER_SEC;
                                printf("时间开销time=%ldms\n", cost);
                                getchar();

                                getchar();
                                break;
                            case 5:
                                if (SaveSolution(flag, truthtable, cost))
                                    printf("SAT的解保存成功!(位于solution.txt文件中)\n");
                                else
                                    printf("SAT的解保存失败!\n");
                                getchar(); getchar();
                                break;
                            case 6:
                                truthtable = (int*)malloc(var_num * sizeof(int));
                                start_t = clock();
                                DPLL_og(&S, 0, 0, truthtable);
                                end_t = clock();
                                clock_t t = (end_t - start_t);
                                ReadFile(&S, filename);
                                start_t = clock();
                                DPLL(&S, 0, 0, truthtable);
                                end_t = clock();
                                clock_t t0 = end_t - start_t;
                                double percent = 100 * (1.0 * (t - t0) / (1.0 * t));
                                printf("优化效率ρ=(t-t0)/t=%.1lf%%\n", percent);
                                getchar();
                                getchar();
                            }
                        }
                    }
                    break;
                case 2:
                    while (op_sudo)
                    {
                        system("cls");
                        printf("\n              Menu for SUDO                \n");
                        printf("-------------------------------------------\n");
                        printf("     1.choose puzzle           2.get answer  \n");
                        printf("     3.play                      \n");
                        printf("     0.exit                      \n");
                        printf("--------------------------------------------\n");
                        if (scanf("%d", &op_sudo))
                        {
                            switch (op_sudo)
                            {
                            case 1:
                                printf("默认数独阶数为6\n");
                                order = 6;
                                var_num = 0;
                                sen_num = 0;
                                truthtable = (int*)malloc(30000 * sizeof(int));
                                for (int i = 0; i < 30000; i++)
                                {
                                    truthtable[i] = -1; //初始化真值表
                                }
                                int sudo_table[6][6];
                                ChoosePuzzle(truthtable);
                                SudoTransfer(order);
                                ReleaseLink(&S);
                                if (ReadFile(&S, target_filename))
                                    printf("读取文件成功!\n");
                                for (int count = 0; count < order * order; count++)
                                {
                                    sudo_table[count / order][count % order] = truthtable[count];
                                }
                                for (int i = 0; i < order * order; i++)
                                {
                                    if (truthtable[i] == -1)
                                        continue;
                                    if (truthtable[i] == 0)
                                    {

                                        Del_CwithL(&S, -(i + 1));
                                        Del_LinC(&S, i + 1);
                                    }
                                    if (truthtable[i] == 1)
                                    {
                                        Del_CwithL(&S, i + 1);
                                        Del_LinC(&S, -(i + 1));
                                    }
                                }
                                getchar();
                                getchar();
                                break;
                            case 2:
                                printf("即将启动DPLL算法求解SAT问题\n");
                                start_t = clock();
                                if (DPLL(&S, 0, 0, truthtable))
                                {
                                    int count = 0;
                                    printf("--------有解且解如下所示-------\n");
                                    //传值给数独数组
                                    for (int count = 0; count < order * order; count++)
                                    {
                                        sudo_table[count / order][count % order] = truthtable[count];
                                    }
                                    ShowPuzzle(sudo_table, order);
                                }
                                else
                                    printf("-------------无解--------------\n");
                                end_t = clock();
                                cost = 1000 * (end_t - start_t) / CLOCKS_PER_SEC;
                                printf("时间开销time=%ldms\n", cost);
                                getchar();
                                getchar();
                                break;
                            case 3:
                                while (!IfCompleted(sudo_table, order))
                                {
                                    system("cls");
                                    ShowPuzzle(sudo_table, order);
                                    printf("请输入行和列的值:\n");
                                    scanf("%d %d", &row, &col);
                                    printf("请输入要填写的真值:(0/1)\n");
                                    scanf("%d", &truth);
                                    sudo_table[row - 1][col - 1] = truth;
                                }
                                printf("finished!Now please Reload the sudo and check your answer!\n");
                                getchar(); getchar();
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
