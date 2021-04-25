#include<iostream>
#include<stdlib.h>
#include<stdio.h>

#define Free 0 //����״̬
#define Busy 1 //����״̬
#define OK 1    //���
#define ERROR 0 //����
typedef int Status;
int flag;

/**
 *���ÿ��п�
 */
typedef struct freearea
{
    int address;//��������ʼ��ַ
    int size;//��ҵ�ռ��С
    int state;//���п�״̬
} ElemType;
/**
 *�������ű�ֱ����ڴ���ڴ�����Ϳ��б�
 *�ڴ�����Ϊ*DuLinkList2,���б�Ϊ*DuLinkList1
 */
typedef struct DuLNode
{
    ElemType data;
    struct DuLNode *prior;
    struct DuLNode *next;
} DuLNode, *DuLinkList1,*DuLinkList2;
/**
 *block_first
 *�����׵�ַ
 *block_last
 *����β��ַ
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
 *���̷����DuLinkList2�ĳ�ʼ��
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
 *��ʼ�����б�DuLinkList1
 *�����û������õ��ڴ��СMAX_length
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
 *�����������Ŀ��п��С
 *���÷�����п��С���㷨worst
 */
void alloc()
{
    int request;//�û����������
    printf("������������������Ŀռ��С��");
    scanf("%d",&request);
    if(request<=0)
    {
        printf("��������ڴ��С����С�ڵ���0 ����������");
    }
    else
    {
        if(worst(request)==OK)//�ж����Ӧ�㷨�Ƿ�ʵ��
        {
            printf("����ɹ���");
        }
        else
        {
            printf("�ڴ治�����ʧ�ܣ�");
        }
    }


}
/**
 *���Ӧ�㷨
 *�ڿ��б��������һ�����п����
 *���б��Ѿ�����С�����˳������
 */
Status worst(int request)
{
    DuLinkList2 temp = (DuLinkList2)malloc(sizeof(DuLNode));//Ϊ���̷����������ָ������ڴ�ռ�
    temp->data.size=request;//���½��̵Ŀռ估״̬����������ָ��
    temp->data.state=Busy;
    DuLNode *p = block_last1;//p��Ϊ�˲��ҷ����½��������������С�ı���ָ��
    DuLNode *p1 = block_first2->next;//p1��Ϊ�˽�������ָ����뵽���̷������
    while(p)
    {
        if(request<(p->data.size))//�ڿ����������Ƿ���ڴ���������̴�С�Ŀ�����
        {
            temp->prior=p1->prior;//�ҵ����·���Ľ�����Ϣ���뵽���̷������׵�ַ
            temp->next=p1;
            temp->data.address=p->data.address;
            p1->prior->next=temp;
            p1->prior=temp;
            p->data.address=temp->data.address+temp->data.size;
            p->data.size-=request;
            return OK;
            break;


        }
        else if(request==(p->data.size))//�����Ƿ��п��������ڵ�����������ڴ�Ľ��̴�С
        {
            p->prior->next=p->next;//�ҵ����ķ���Ľ�����Ϣ���뵽���̷������
            temp->prior=p1->prior;
            temp->next=p1;
            temp->data.address=p->data.address;
            p1->prior->next=temp;
            p1->prior=temp;
            return OK;
            break;
        }
        else//ǰ�����������û�ҵ���ֱ������ڴ治��
        {
           return ERROR;
        }
    }
}
/**
 *�����ڴ��㷨
 *��Ҫ���յĽ��̿�Ĵ�С����б���������ν��бȽ�
 *��С����嵽ǰ��
 *����βָ�����ֱ�ӷŵ�βָ���
 */
void free(int flag)
{
    DuLinkList1 temp = (DuLinkList2)malloc(sizeof(DuLNode));//���շ�����еĽ���ʱ��Ҫ�ڿ��б�������һ��ָ��
    DuLNode *p = block_first2;//Ϊ�˱���Ѱ���û�����Ľ����յĽ���
    DuLNode *q = block_first1->next;//Ϊ��Ѱ�Ҵ��ڵ��ڲ����ָ���С��ָ��
    for(int i=0; i<=flag; i++)
    {
        if(p!=NULL)
        {
            p=p->next;
        }
    }//�ҵ����û���Ҫ�Ļ��յĽ���
    temp->data.address=p->data.address;
    temp->data.size=p->data.size;
    temp->data.state=Free;
    if(p!=block_first2&&p!=block_last2)
    {
           p->prior->next=p->next;//ɾ�����̱��б����յĽ���
    }
    else if(p==block_last2)
    {
        p->prior->next=NULL;
    }


    while(q)
    {
        if(q->data.size>=temp->data.size)//�����һ��������Ĵ�СС�ڵ��ڻ��յĽ��̿ռ��С�򽫻��յĽ��̿ռ�ŵ��׵�ַ
        {
            temp->prior=q->prior;
            q->prior->next=temp;
            temp->next=q;
            q->prior=temp;
            break;
        }
        else if(q!=block_last1&&q->data.size<temp->data.size)//�����һ��������Ĵ�С���ڻ��յĽ��̿ռ��Ҹÿ��п鲻��βָ���������һ��
        {
            q=q->next;
        }
        //have a question
        else if(q==block_last1&&q->data.size<temp->data.size)//�����һ��������Ĵ�С���ڻ��յĽ��̿ռ��Ҹÿ��п���βָ���򽫻��յĽ��̿ռ���뵽βָ��֮��
        {
            temp->prior=block_last1;
            block_last1->next=temp;
            temp->next=NULL;
            break;
        }
    }
}
/**
 *��ʾ���б���亯��
 *���������ָ�뿪ʼ
 *������ʾ
 */
void show1()
{
    int flag=0;
    printf("������������\n");
    DuLNode *q=block_first1->next;
    printf("������\t��ʼ��ַ ������С\t״̬\n\n");
    while(q)
    {
        printf("%d\t",flag);
        flag++;
        printf("%d\t",q->data.address);
        printf("%dKB\t",q->data.size);
        if(q->data.state==Free)
        {
            printf("      ����\n\n");
        }
        else
        {
            printf("      �ѷ���\n\n");
        }
        q=q->next;
    }
    printf("++++++++++++++++++++++++++++\n");
}
/**
 *��ʾ���̷������
 *���������ָ�뿪ʼ
 *������ʾ
 */
void show2()
{
    int flag=0;
    printf("������������\n");
    DuLNode *q=block_first2->next;
    printf("������\t��ʼ��ַ ������С\t״̬\n\n");
    while(q&&q!=block_last2)
    {
        printf("%d\t",flag);
        flag++;
        printf("%d\t",q->data.address);
        printf("%dKB\t",q->data.size);
        if(q->data.state==Free)
        {
            printf("      ����\n\n");
        }
        else
        {
            printf("      �ѷ���\n\n");
        }
        q=q->next;
    }
    printf("++++++++++++++++++++++++++++\n");
}
int main()
{
    int c=1;
    int MAX_length;
    printf("**********�����ڲ����������Ӧ�����㷨*****************\n");
    printf("***�������ʼ����Ƭ�Ĵ�С��");
    scanf("%d",&MAX_length);
    initblock(MAX_length);
    initblock();
    int choice;
    while(c==1)
    {
        printf("\n1: �����ڴ�   2:�����ڴ�  3:�˳�ϵͳ  4:��ʾ����������� 5����ʾ�����������\n");
        printf("*******�������������еĲ�����");
        scanf("%d",&choice);
        if(choice==1)
        {
            alloc();
            c=1;
        }
        else if(choice==2)
        {
            printf("����������Ҫ���յĿ��п����ţ�");
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
            printf("���������� ���������룡");
            c=1;
        }
    }
    return 0;
}