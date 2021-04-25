//#include "stdafx.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXJOB 200 // 定义存储数据最大值

typedef struct node // 内存块结构
{
  int start;
  int length;
  char tag[20];
}job;

job frees[MAXJOB]; // 定义空闲区表
int free_quantity; // 空闲区块数

job occupys[MAXJOB]; // 定义已分配区表
int occupy_quantity; // 已分配区块数

 // 初始化函数

void initial()

{

       int i;

       for (i = 0; i < MAXJOB; i++)

       {

              frees[i].start = -1;

              frees[i].length = 0;

              strcpy(frees[i].tag, "free"); // 定义所有块为空闲块

              occupys[i].start = -1;

              occupys[i].length = 0;

              strcpy(occupys[i].tag, ""); // 已分配块为 0

       }

       free_quantity = 0;

       occupy_quantity = 0;

}

// 读数据函数，从文件中读入空闲表的设置

int readData()

{

       FILE * fp;

       if ((fp = fopen("D:\\mem.txt", "r")) == NULL)

       {

              printf(" 错误, 文件不存在\n");

       }

       else

       {

              while (!feof(fp))

              { // 打开文件读入空闲表信息

                     fscanf(fp, "%d %d", & frees[free_quantity].start, & frees[free_quantity].length);

                     free_quantity++;

              }

              return 1;

       }

       return 0;

}

// 排序空闲表

void sort()

{

       int i, j, p;

       for (i = 0; i < free_quantity - 1; i++)

       {

              p = i;

              for (j = i + 1; j < free_quantity; j++)

              {

                     if (frees[j].start < frees[p].start)

                     {

                            p = j;

                     }

              }

              if (p != i)

              {

                     frees[free_quantity] = frees[i];

                     frees[i] = frees[p];

                     frees[p] = frees[free_quantity];

              }

       }

}

// 显示函数

void view()

{

       int i;

       printf(" 空闲表 :\n"); // 显示空闲表

       printf(" 起始地址   长度  状态\n");

       for (i = 0; i < free_quantity; i++)

       {

              printf("%8d\t%4d\t%s\n", frees[i].start, frees[i].length, frees[i].tag);

       }

 

       getchar();

}

void prin()

{

       printf(" 当前已分配表 :\n");

       printf(" 起始地址   长度  占用作业名 \n");

       for (int i = 0; i < occupy_quantity; i++)

       { // 显示已分配表

              printf("%8d\t%4d\t%s\n", occupys[i].start, occupys[i].length, occupys[i].tag);

       }

}

// 最先适应分配算法

void earliest()

 {

       char job_name[10];

       int job_length;

       int i, j, flag, t;

       printf(" 请输入新申请内存空间的作业名 :");

       scanf("%s", & job_name);

       printf(" 请输入新申请内存空间的大小 :");

       scanf("%d", & job_length);

       flag = 0;

       for (i = 0; i < free_quantity; i++)

       { // 顺序查找满足条件的空间

              if (frees[i].length >= job_length)

              {

                     flag = 1;

              }

       }

       if (flag == 0)

       { // 没找到满足的空间

              printf("\n 当前没有能满足你申请长度的空闲内存 , 请稍候再试 \n");

       }

       else

       { // 找到了满足的空间

              t = 0;

              i = 0;

              while (t == 0)

              {

                     if (frees[i].length >= job_length)

                     {

                            t = 1;

                     }

                     i++;

              }

              i--;

              occupys[occupy_quantity].start = frees[i].start; // 分配满足条件的空间

              strcpy(occupys[occupy_quantity].tag, job_name);

              occupys[occupy_quantity].length = job_length;

              occupy_quantity++;

              if (frees[i].length > job_length)

              {

                     frees[i].start += job_length;

                     frees[i].length -= job_length;

              }

              else

              {

                     for (j = i; j < free_quantity - 1; j++)

                     {

                            frees[j] = frees[j + 1];

                     }

                     free_quantity--;

                     printf(" 内存空间成功 !\n");

              }

       }

}

// 最优适应分配算法

void excellent()

 {

       char job_name[20];

       int job_length;

       int i, j, flag, t;

       printf(" 请输入新申请内存空间的作业名 :");

       scanf("%s", & job_name);

       printf(" 请输入新申请内存空间的大小 :");

       scanf("%d", & job_length);

       flag = 0;

       for (i = 0; i < free_quantity; i++)

       {

              if (frees[i].length >= job_length)

              {

                     flag = 1;

              }

       }

       if (flag == 0)

       {

              printf("\n 当前没有能满足你申请长度的空闲内存 , 请稍候再试 \n");

       }

       else

       {

              t = 0;

              i = 0;

 

              while (t == 0)

              {

                     if (frees[i].length >= job_length)

                     {

                            t = 1;

                     }

                     i++;

              }

              i--;

              for (j = 0; j < free_quantity; j++)

              { // 找到满足条件的最小空闲空间

                     if ((frees[j].length >= job_length) && (frees[j].length < frees[i].length))

                     {

                            i = j;

                     }

              }

              occupys[occupy_quantity].start = frees[i].start; // 分配空闲空间

              strcpy(occupys[occupy_quantity].tag, job_name);

              occupys[occupy_quantity].length = job_length;

              occupy_quantity++;

              if (frees[i].length > job_length)

              {

                     frees[i].start += job_length;

                     frees[i].length -= job_length;

              }

              else

              {

                     for (j = i; j < free_quantity - 1; j++)

                     {

                            frees[j] = frees[j + 1];

                     }

                     free_quantity--;

                     printf(" 内存空间成功 !\n");

              }

       }

}

// 最坏适应算法

void worst()

 {

       char job_name[20];

       int job_length;

       int i, j, flag, t;

       printf(" 请输入新申请内存空间的作业名 :");

       scanf("%s", & job_name);

       printf(" 请输入新申请内存空间的大小 :");

       scanf("%d", & job_length);

       flag = 0;

       for (i = 0; i < free_quantity; i++)

       {

              if (frees[i].length >= job_length)

              {

                     flag = 1;

              }

       }

       if (flag == 0)

       {

              printf("\n 当前没有能满足你申请长度的空闲内存 , 请稍候再试 \n");

       }

       else

       {

              t = 0;

              i = 0;

              while (t == 0)

              {

                     if (frees[i].length >= job_length)

                     {

                            t = 1;

                     }

                     i++;

              }

              i--;

              for (j = 0; j < free_quantity; j++)

              { // 找到满足条件的最大空闲空间

                     if ((frees[j].length >= job_length) && (frees[j].length > frees[i].length))

                     {

                            i = j;

                     }

              }

              occupys[occupy_quantity].start = frees[i].start; // 分配空闲空间

              strcpy(occupys[occupy_quantity].tag, job_name);

              occupys[occupy_quantity].length = job_length;

              occupy_quantity++;

              if (frees[i].length > job_length)

              {

                     frees[i].start += job_length;

                     frees[i].length -= job_length;

              }

              else

              {

                     for (j = i; j < free_quantity - 1; j++)

                     {

                            frees[j] = frees[j + 1];

                     }

                     free_quantity--;

                     printf(" 内存空间成功 !\n");

              }

       }

}

// 主函数

int main()

{

       int flag = 0;
       int t = 1;
       int chioce = 0;
       printf("this is the beginning");
       initial(); // 初始化
       flag = readData(); // 读空闲表文件

       while (flag == 1)
       {
              sort(); // 排序

              // 显示菜单
              printf(" 动态分区算法 \n");
              printf("1. 显示空闲表和分配表 \n2. 最先适应法 \n3. 最优适应法 \n4. 最坏适应法 \n0. 退出\n");
              printf(" 请选择 :");
              scanf("%d", & chioce); // 输入选择的菜单项
              switch (chioce)
              {
              case 1:
                     // 显示空闲表和分配表
                     view();
                     break;
              case 2:
                     // 调用最先适应法
                     earliest();
                     prin();
                     break;
              case 3:
                     // 最优适应法
                     excellent();
                     prin();

                     break;

              case 4:

                     // 最坏适应法

                     worst();

                     prin();

                     break;

              case 0:

                     // 退出

                     flag = 0;

                     break;

              default:

                     printf("\n 选择错误 !\n");

              }

       }
       system("pause");
       return 0;
}