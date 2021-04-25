#include<iostream>
#include<stdlib.h>
#include<stdio.h>

#define Free 0 //空闲状态
#define Busy 1 //已用状态
#define OK 1    //完成
#define ERROR 0 //出错
typedef int Status;
int flag;

/**
 *设置空闲块
 */
typedef struct freearea
{
    int address;//空闲区起始地址
    int size;//作业空间大小
    int state;//空闲块状态
} ElemType;
/**
 *定义俩张表分别用于存放内存分配表和空闲表
 *内存分配表为*DuLinkList2,空闲表为*DuLinkList1
 */
typedef struct DuLNode
{
    ElemType data;
    struct DuLNode *prior;
    struct DuLNode *next;
} DuLNode, *DuLinkList1,*DuLinkList2;
/**
 *block_first
 *链表首地址
 *block_last
 *链表尾地址
 */
DuLinkList1 block_first1;
DuLinkList1 block_last1;
DuLinkList2 block_first2;
DuLinkList2 block_last2;
void alloc(int);
void free(int);
Status worst(int);
void show();
/**
 *进程分配表DuLinkList2的初始化
 */
void initblock()
{
    block_first2=(DuLinkList2)malloc(sizeof(DuLNode));
    block_last2=(DuLinkList2)malloc(sizeof(DuLNode));
    block_first2->prior=NULL;
    block_first2->next=block_last2;
    block_last2->prior=block_first2;
    block_last2->next=NULL;
}
/**
 *初始化空闲表DuLinkList1
 *传入用户需设置的内存大小MAX_length
 */
void initblock(int MAX_length)
{
    block_first1=(DuLinkList1)malloc(sizeof(DuLNode));
    block_last1=(DuLinkList1)malloc(sizeof(DuLNode));
    block_first1->prior=NULL;
    block_first1->next=block_last1;
    block_last1->prior=block_first1;
    block_last1->next=NULL;
    block_last1->data.address=0;
    block_last1->data.size=MAX_length;
    block_last1->data.state=Free;


}
/**
 *输入进程请求的空闲块大小
 *调用分配空闲块大小的算法worst
 */
void alloc()
{
    int request;//用户输入的请求
    printf("请您输入进程所需分配的空间大小：");
    scanf("%d",&request);
    if(request<=0)
    {
        printf("输入错误，内存大小不能小于等于0 请重新输入");
    }
    else
    {
        if(worst(request)==OK)//判断最坏适应算法是否实现
        {
            printf("分配成功！");
        }
        else
        {
            printf("内存不足分配失败！");
        }
    }


}
/**
 *最坏适应算法
 *在空闲表中找最后一个空闲块分配
 *空闲表已经按从小到大的顺序排列
 */
Status worst(int request)
{
    DuLinkList2 temp = (DuLinkList2)malloc(sizeof(DuLNode));//为进程分配表新增的指针分配内存空间
    temp->data.size=request;//把新进程的空间及状态赋给新增的指针
    temp->data.state=Busy;
    DuLNode *p = block_last1;//p是为了查找符合新进程所需空闲区大小的遍历指针
    DuLNode *p1 = block_first2->next;//p1是为了将新增的指针插入到进程分配表中
    while(p)
    {
        if(request<(p->data.size))//在空闲区查找是否存在大于申请进程大小的空闲区
        {
            temp->prior=p1->prior;//找到则将新分配的进程信息插入到进程分配表的首地址
            temp->next=p1;
            temp->data.address=p->data.address;
            p1->prior->next=temp;
            p1->prior=temp;
            p->data.address=temp->data.address+temp->data.size;
            p->data.size-=request;
            return OK;
            break;


        }
        else if(request==(p->data.size))//查找是否有空闲区存在等于申请分配内存的进程大小
        {
            p->prior->next=p->next;//找到则将心分配的进程信息插入到进程分配表中
            temp->prior=p1->prior;
            temp->next=p1;
            temp->data.address=p->data.address;
            p1->prior->next=temp;
            p1->prior=temp;
            return OK;
            break;
        }
        else//前面俩种情况都没找到则直接输出内存不够
        {
           return ERROR;
        }
    }
}
/**
 *回收内存算法
 *将要回收的进程块的大小与空闲表的数据依次进行比较
 *若小于则插到前面
 *若比尾指针大则直接放到尾指针后
 */
void free(int flag)
{
    DuLinkList1 temp = (DuLinkList2)malloc(sizeof(DuLNode));//回收分配表中的进程时需要在空闲表中新增一个指针
    DuLNode *p = block_first2;//为了遍历寻找用户输入的将回收的进程
    DuLNode *q = block_first1->next;//为了寻找大于等于插入的指针大小的指针
    for(int i=0; i<=flag; i++)
    {
        if(p!=NULL)
        {
            p=p->next;
        }
    }//找到了用户需要的回收的进程
    temp->data.address=p->data.address;
    temp->data.size=p->data.size;
    temp->data.state=Free;
    if(p!=block_first2&&p!=block_last2)
    {
           p->prior->next=p->next;//删除进程表中被回收的进程
    }
    else if(p==block_last2)
    {
        p->prior->next=NULL;
    }


    while(q)
    {
        if(q->data.size>=temp->data.size)//如果第一块空闲区的大小小于等于回收的进程空间大小则将回收的进程空间放到首地址
        {
            temp->prior=q->prior;
            q->prior->next=temp;
            temp->next=q;
            q->prior=temp;
            break;
        }
        else if(q!=block_last1&&q->data.size<temp->data.size)//如果第一块空闲区的大小大于回收的进程空间且该空闲块不是尾指针则遍历下一个
        {
            q=q->next;
        }
        //have a question
        else if(q==block_last1&&q->data.size<temp->data.size)//如果第一块空闲区的大小大于回收的进程空间且该空闲块是尾指针则将回收的进程空间插入到尾指针之后
        {
            temp->prior=block_last1;
            block_last1->next=temp;
            temp->next=NULL;
            break;
        }
    }
}
/**
 *显示空闲表分配函数
 *从链表的首指针开始
 *依次显示
 */
void show1()
{
    int flag=0;
    printf("主存分配情况：\n");
    DuLNode *q=block_first1->next;
    printf("分区号\t起始地址 分区大小\t状态\n\n");
    while(q)
    {
        printf("%d\t",flag);
        flag++;
        printf("%d\t",q->data.address);
        printf("%dKB\t",q->data.size);
        if(q->data.state==Free)
        {
            printf("      空闲\n\n");
        }
        else
        {
            printf("      已分配\n\n");
        }
        q=q->next;
    }
    printf("++++++++++++++++++++++++++++\n");
}
/**
 *显示进程分配表函数
 *从链表的首指针开始
 *依次显示
 */
void show2()
{
    int flag=0;
    printf("主存分配情况：\n");
    DuLNode *q=block_first2->next;
    printf("分区号\t起始地址 分区大小\t状态\n\n");
    while(q&&q!=block_last2)
    {
        printf("%d\t",flag);
        flag++;
        printf("%d\t",q->data.address);
        printf("%dKB\t",q->data.size);
        if(q->data.state==Free)
        {
            printf("      空闲\n\n");
        }
        else
        {
            printf("      已分配\n\n");
        }
        q=q->next;
    }
    printf("++++++++++++++++++++++++++++\n");
}
int main()
{
    int c=1;
    int MAX_length;
    printf("**********您现在操作的是最坏适应分配算法*****************\n");
    printf("***请输入初始空闲片的大小：");
    scanf("%d",&MAX_length);
    initblock(MAX_length);
    initblock();
    int choice;
    while(c==1)
    {
        printf("\n1: 分配内存   2:回收内存  3:退出系统  4:显示进程运行情况 5：显示空闲区的情况\n");
        printf("*******请输入您将进行的操作：");
        scanf("%d",&choice);
        if(choice==1)
        {
            alloc();
            c=1;
        }
        else if(choice==2)
        {
            printf("请输入您想要回收的空闲块区号：");
            scanf("%d",&flag);
            free(flag);
            c=1;
        }
        else if(choice==3)
        {
            exit(1);
        }
        else if(choice==4)
        {
            show2();
            c=1;
        }
        else if(choice==5)
        {
            show1();
            c=1;
        }
        else
        {
            printf("操作不存在 请重新输入！");
            c=1;
        }
    }
    return 0;
}