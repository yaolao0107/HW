#include "head.h"
//新增数独规约函数，将数独转化为子句集形式，并保存为cnf文件,文件名需要输入
status SudoTransfer(int order)
{
    FILE *fp = NULL;
    int count_row = 0, count_col = 0;
    ElemType *data = NULL;
    char filename[50];
    strcpy(filename,"D:\\tst.txt");
    fp = fopen(filename, "w");
    var_num = order * order;
    sen_num = 0;
    fprintf(fp, "p cnf 30000 30000\n");
    for (count_row = 0; count_row < order; count_row++)
    {
        for (count_col = 0; count_col < order - 2; count_col++)
        {
            int var = order * count_row + count_col + 1; //ij
            fprintf(fp, "%d %d %d 0\n", var, var + 1, var + 2);
            fprintf(fp, "%d %d %d 0\n", -var, -(var + 1), -(var + 2));
            sen_num += 2;
        }
    }
    for (count_col = 0; count_col < order; count_col++)
    {
        for (count_row = 0; count_row < order - 2; count_row++)
        {
            int var = order * count_row + count_col + 1; //ij
            fprintf(fp, "%d %d %d 0\n", var, var + order, var + 2 * order);
            fprintf(fp, "%d %d %d 0\n", -var, -(var + order), -(var + 2 * order));
            sen_num += 2;
        }
    }
    //规则2
    int *combine = (int *)malloc(order * sizeof(int));
    int *target = (int *)malloc(order * sizeof(int));
    for (count_row = 0; count_row < order; count_row++)
    {
        //设定combine数组的起始状态
        for (count_col = 0; count_col < order / 2 + 1; count_col++)
            combine[count_col] = 1;
        for (; count_col < order; count_col++)
            combine[count_col] = 0;
        //设定target数组的内容
        for (count_col = 0; count_col < order; count_col++)
            target[count_col] = order * count_row + count_col + 1; //ij
        data = (int *)malloc((order / 2 + 1) * sizeof(int));
        while (UniqueCombine(combine, target, data, order))
        {
            for (int count = 0; count < order / 2 + 1; count++)
            {
                fprintf(fp, "%d ", data[count]);
            }
            fprintf(fp, "0 \n");
            for (int count = 0; count < order / 2 + 1; count++)
            {
                fprintf(fp, "%d ", -data[count]);
            }
            fprintf(fp, "0 \n");
            sen_num++;
        }
        for (int count = 0; count < order / 2 + 1; count++)
        {
            fprintf(fp, "%d ", data[count]);
        }
        fprintf(fp, "0 \n");
        for (int count = 0; count < order / 2 + 1; count++)
        {
            fprintf(fp, "%d ", -data[count]);
        }
        fprintf(fp, "0 \n");
        sen_num++;
    }
    for (count_col = 0; count_col < order; count_col++)
    {
        //设定combine数组的起始状态
        for (count_row = 0; count_row < order / 2 + 1; count_row++)
            combine[count_row] = 1;
        for (; count_row < order; count_row++)
            combine[count_row] = 0;
        //设定target数组的内容
        for (count_row = 0; count_row < order; count_row++)
            target[count_row] = count_row * order + count_col + 1;
        data = (int *)malloc((order / 2 + 1) * sizeof(int));
        while (UniqueCombine(combine, target, data, order))
        {
            for (int count = 0; count < order / 2 + 1; count++)
            {
                fprintf(fp, "%d ", data[count]);
            }
            fprintf(fp, "0 \n");
            for (int count = 0; count < order / 2 + 1; count++)
            {
                fprintf(fp, "%d ", -data[count]);
            }
            fprintf(fp, "0 \n");
            sen_num++;
        }
        //曾经漏掉了这一个，因为跳出循环的条件是没用10组合，但是最后一组data这时还储存在数组里面，要打印出来
        for (int count = 0; count < order / 2 + 1; count++)
        {
            fprintf(fp, "%d ", data[count]);
        }
        fprintf(fp, "0 \n");
        for (int count = 0; count < order / 2 + 1; count++)
        {
            fprintf(fp, "%d ", -data[count]);
        }
        fprintf(fp, "0 \n");
    }
    //规则3
    int var1, var2;
    int extra_var_true[6], extra_var_false[6];
    int extra_var_same[6];
    //基准行
    for (int base_line = 0; base_line < order; base_line++)
    {
        //比较行
        for (int cmp_line = base_line + 1; cmp_line < order; cmp_line++)
        {
            //每一列
            for (count_col = 0; count_col < order; count_col++)
            {
                var1 = order * base_line + count_col + 1;                                                    //51
                var2 = order * cmp_line + count_col + 1;                                                     //71
                extra_var_true[count_col] = 10000 + 1000 * base_line + 100 * cmp_line + 10 * count_col + 1;  //15711
                extra_var_false[count_col] = 10000 + 1000 * base_line + 100 * cmp_line + 10 * count_col + 0; //15710
                extra_var_same[count_col] = 1000 + 100 * base_line + 10 * cmp_line + count_col;              //1571
                var_num += 3;
                //15711= 51∧71 转化为 CNF 时为（51 ∨ -15711）∧（71 ∨ -15711）∧（-51 ∨ -71 ∨ 15711）
                fprintf(fp, "%d %d 0 \n", -extra_var_true[count_col], var1);
                fprintf(fp, "%d %d 0 \n", -extra_var_true[count_col], var2);
                fprintf(fp, "%d %d %d 0 \n", extra_var_true[count_col], -var1, -var2);
                sen_num += 3;
                //15720= -52∧-72 转化为 CNF 时为（-52 ∨ -15720）∧（-72 ∨ -15720）∧（52 ∨ 72 ∨ 15720）
                fprintf(fp, "%d %d 0 \n", -extra_var_false[count_col], -var1);
                fprintf(fp, "%d %d 0 \n", -extra_var_false[count_col], -var2);
                fprintf(fp, "%d %d %d 0 \n", extra_var_false[count_col], var1, var2);
                sen_num += 3;
                //1578= 15781 ∨ 15780 转化为 CNF 时为（-15781 ∨ 1578）∧（-15780 ∨ 1578）∧（15781 ∨ 15780 ∨ -1578）
                fprintf(fp, "%d %d 0 \n", -extra_var_true[count_col], extra_var_same[count_col]);
                fprintf(fp, "%d %d 0 \n", -extra_var_false[count_col], extra_var_same[count_col]);
                fprintf(fp, "%d %d %d 0 \n", extra_var_true[count_col], extra_var_false[count_col], -extra_var_same[count_col]);
                sen_num += 3;
            }
            int var3 = 100 + 10 * base_line + cmp_line; //157

            //157= -[1571∧1572∧…∧1578] 转化为 CNF 时为（-157∨-1571∨-1572∨…∨-1578）∧（1571∨157）∧（1572∨157）…（1578∨157）
            for (int count = 0; count < order; count++)
                fprintf(fp, "%d ", -extra_var_same[count]);
            fprintf(fp, "%d 0 \n", -var3);
            sen_num++;
            for (int count = 0; count < order; count++)
            {
                fprintf(fp, "%d %d 0 \n", extra_var_same[count], var3);
                sen_num++;
            }
        }
    }

    //基准列
    for (int base_col = 0; base_col < order; base_col++)
    {
        //比较列
        for (int cmp_col = base_col + 1; cmp_col < order; cmp_col++)
        {
            //每一行
            for (count_row = 0; count_row < order; count_row++)
            {
                var1 = order * count_row + base_col + 1;                                                   //15 25
                var2 = order * count_row + cmp_col + 1;                                                    //17 27
                extra_var_true[count_row] = 20000 + 1000 * base_col + 100 * cmp_col + 10 * count_row + 1;  //25711
                extra_var_false[count_row] = 20000 + 1000 * base_col + 100 * cmp_col + 10 * count_row + 0; //25710
                extra_var_same[count_row] = 2000 + 100 * base_col + 10 * cmp_col + count_row;              //2571
                var_num += 3;
                //25711= 15∧17 转化为 CNF 时为（15 ∨ -25711）∧（17 ∨ -25711）∧（-15 ∨ -17 ∨ 25711）
                fprintf(fp, "%d %d 0 \n", -extra_var_true[count_row], var1);
                fprintf(fp, "%d %d 0 \n", -extra_var_true[count_row], var2);
                fprintf(fp, "%d %d %d 0 \n", extra_var_true[count_row], -var1, -var2);
                sen_num += 3;
                //25720= -25∧-27 转化为 CNF 时为（-25 ∨ -25720）∧（-27 ∨ -20-9  5720）∧（25 ∨ 27 ∨ 25720）
                fprintf(fp, "%d %d 0 \n", -extra_var_false[count_row], -var1);
                fprintf(fp, "%d %d 0 \n", -extra_var_false[count_row], -var2);
                fprintf(fp, "%d %d %d 0 \n", extra_var_false[count_row], var1, var2);
                sen_num += 3;
                //2578= 25781 ∨ 25780 转化为 CNF 时为（-25781 ∨ 2578）∧（-25780 ∨ 2578）∧（25781 ∨ 25780 ∨ -2578）
                fprintf(fp, "%d %d 0 \n", -extra_var_true[count_row], extra_var_same[count_row]);
                fprintf(fp, "%d %d 0 \n", -extra_var_false[count_row], extra_var_same[count_row]);
                fprintf(fp, "%d %d %d 0 \n", extra_var_true[count_row], extra_var_false[count_row], -extra_var_same[count_row]);
                sen_num += 3;
            }
            int var3 = 200 + 10 * base_col + cmp_col; //257

            //257= -[2571∧2572∧…∧2578] 转化为 CNF 时为（-257∨-2571∨-2572∨…∨-2578）∧（2571∨257）∧（2572∨257）…（2578∨257）
            for (int count = 0; count < order; count++)
                fprintf(fp, "%d ", -extra_var_same[count]);
            fprintf(fp, "%d 0 \n", -var3);
            sen_num++;
            for (int count = 0; count < order; count++)
            {
                fprintf(fp, "%d %d 0 \n", extra_var_same[count], var3);
                sen_num++;
            }
        }
    }
    strcpy(target_filename, filename);
    fclose(fp);
    return TRUE;
}

//新增从n个元素中选取m个元素的全组合函数(10转换法)
//函数功能是把combine对应的target数组中的内容压缩到data数组中，并通过改变10组合产生一种新的不重复的组合
//combine数组中改变过10状态返回1，否则返回0,使用前需要将target数组中m个1推给target数组靠左的位置,以设定启动状态
int UniqueCombine(int *combine, int *target, int *data, int target_num)
{
    //flag用来判定是否更替过10组合
    int count = 0, num1_num = 0;
    //读入当前组合到data中
    for (int i = 0; count < target_num; count++)
    {
        if (combine[count])
        {
            data[i] = target[count];
            i++;
        }
    }
    //通过改变第一个10组合更新combine
    for (count = 0; count < target_num - 1; count++)
    {
        //检索第一个10组合
        if (combine[count] && !combine[count + 1])
        {
            //将10变成01
            combine[count] = 0;
            combine[count + 1] = 1;
            //统计01组合左侧1的个数
            for (int i = 0; i < count; i++) //曾经发生计数的错误
                if (combine[i])
                    num1_num++;
            //移动所有左边的1到最左侧
            for (int i = 0; i < num1_num; i++)
                combine[i] = 1;
            for (int i = num1_num; i < count; i++) //曾经发生过同上面一样的循环计数错误
                combine[i] = 0;
            return 1;
        }
    }
    return 0;
}
//新增数独题目选择函数
//输入真值表数组，按输入读取对应的puzzle，将前置条件替换
void ChoosePuzzle(int truthtable[])
{
    char dif[10];
    char string[10];
    int choice;
    int op;

    //选择难度
    printf("\nSelect the difficulty!\n");
    printf("1.easy    2.medium    3.hard\n");
    scanf("%d", &op);
    switch (op)
    {
    case 1:
        strcpy(dif, "easy");
        break;
    case 2:
        strcpy(dif, "medium");
        break;
    case 3:
        strcpy(dif, "hard");
        break;
    }

    //选择题号
    printf("\nPlease choose the puzzle(1-10):\n");
    scanf("%d", &choice);

    FILE *fp;
    fp = fopen("D:\\VSCODE\\puzzles.txt", "r");
    if (!fp)
    {
        printf("File open error!\n");
        return;
    }
    //找到题目
    while (1)
    {
        fscanf(fp, "%s", string);
        if (strcmp(string, dif) == 0) //读到对应难度退出循环
            break;
    }
    while (1)
    {
        fscanf(fp, "%d", &op);
        if (op == choice) //读到对应题号退出循环
            break;
    }

    //题目写入内存
    int var = 0;
    while (TRUE)
    {
        fscanf(fp, "%d", &var);
        if (var == 0)
        {
            break;
        }
        if (var > 0)
        {
            int v = (var / 10 - 1) * 6 + var % 10;
            Assert(v, 1, truthtable);
        }
        else
        {
            int v = (abs(var) / 10 - 1) * 6 + abs(var) % 10;
            Assert(v, 0, truthtable);
        }
    }
    fclose(fp);
}

status ShowPuzzle(int sudo_table[6][6],int order)
{
    //打印数独
    for (int count_row = 0; count_row < order; count_row++)
    {
        for (int i = 0; i < order; i++)
            printf("---");
        printf("\n");
        for (int count_col = 0; count_col < order; count_col++)
        {
            printf("|%2d", sudo_table[count_row][count_col]);
        }
        printf("|\n");
    }
    for (int i = 0; i < order; i++)
        printf("---");
    return 0;
}
//当数独填充完成，即无未标记状态时，返回1
status IfCompleted(int sudo_table[6][6],int order)
{
    for (int i = 0; i < order; i++)
    {
        for (int j = 0; j < order; j++)
        {
            if(sudo_table[i][j]==-1)
            return 0;
        }
    }
    return 1; 
}
