#include <stdio.h>
// #include <math.h> 
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stack>
using namespace std;
 
const int Maxn=110;
const int maxn=20;
char str[maxn][Maxn];//输入文法
char fin[maxn];//存储终结符
char firstvt[maxn][maxn],lastvt[maxn][maxn];
int firstflag[maxn],lastflag[maxn];//非终结符的firstvt,lastvt是否求出
int fcnt[maxn],lcnt[maxn];//
/*xyx*/
bool F[maxn][maxn];//table
int NTcount=0;
int Tcount=0;
char NT[maxn];
char T[maxn];
void out_firstvt();
// stack<char,char> STACK;
typedef struct pair_{
    char NT;
    char T;
}pair_;

stack<pair_*> STACK;

// void insert(char NT, char T){
//     // if(F[])
// }
int is_fin(char c) { //判断终结符
    for(int i=0; fin[i]!='\0'; i++) {
        if(fin[i]==c)
            return 1;
    }
    return 0;
}

bool insert(int NTsite, int Tsite){
    if(!F[NTsite][Tsite]){
        F[NTsite][Tsite]=1;
        pair_ *node = (pair_*)malloc(sizeof(pair_)) ;
        node->NT = NT[NTsite];
        node->T = T[Tsite];
        STACK.push(node);
        return 1;
    }
    return 0;
}

int getsite(char ch){
    for(int i=0;i<Tcount;i++){
        if(T[i]==ch){
            return i;
        }
    }
    cout<<"找不到该终结符"<<ch<<"的位置。";
    return -1;
}

void do_firstvt(){ //char s,int t
    int i,j,ii,jj,tt;
    // for(i=0; i<t; i++) {//找到该非终结符对应的产生式
    //     if(str[i][0]==s)
    //         break;
    // }
    for(int i=0; i<NTcount;i++){ //对于每条产生式
        for(j=0; str[i][j]!='\0'; j++) { //遍历这个产生式
            if(j==2||str[i][j]=='|') { //跳过‘s->’的第一个候选式，下一个候选式
                if(is_fin(str[i][j+1])) { //形如s->a，以终结符开头
                    insert(i,getsite(str[i][j+1]));
                } else { //以非终结符开头
                    if(is_fin(str[i][j+2])) { //形如s->Ba,以非终结符开头，且紧跟着终结符
                        insert(i,getsite(str[i][j+2]));
                    }
                }
            }
        }
    }
    out_firstvt();
    char Q,a;
    while(!STACK.empty()){
        pair_ *node = STACK.top() ;
        Q = node->NT;
        a = node->T;
        STACK.pop();
        for(int i=0; i<NTcount;i++){ //对于每条产生式
            for(j=0; str[i][j]!='\0'; j++) { //遍历这个产生式
                if(j==2||str[i][j]=='|') { //跳过‘s->’的第一个候选式，下一个候选式
                    if(Q == str[i][j+1]) { //形如s->a，以终结符开头
                        insert(i,getsite(a));
                }
            }
        }
    }
}}
 //输出firstvt集

void out_firstvt(){
    for(int i=0;i<Tcount;i++){
        cout<<"   "<<T[i];
    }
    cout<<endl;
    for(int i=0;i<NTcount;i++){
        cout<<NT[i]<<"  ";
        for(int j=0;j<Tcount;j++){
            cout<<F[i][j]<<"   ";
        }
        cout<<endl;
    }
}

void get_firstvt(char s,int t) { //求s非终结符的firstvt值
    int i,j,ii,jj,tt;
    for(i=0; i<t; i++) {//找到该非终结符对应的产生式
        if(str[i][0]==s)
            break;
    }
    if(!firstflag[i]) {
        int k=fcnt[i];
        for(j=0; str[i][j]!='\0'; j++) { //遍历这个产生式
            if(j==2||str[i][j]=='|') { //跳过‘s->’的第一个候选式，下一个候选式
                if(is_fin(str[i][j+1])) { //形如s->a，以终结符开头
                    firstvt[i][k++]=str[i][j+1];
                } else { //以非终结符开头
                    if(is_fin(str[i][j+2])) { //形如s->Ba,以非终结符开头，且紧跟着终结符
                        firstvt[i][k++]=str[i][j+2];
                    }
                    if(str[i][j+1]!=s) { //如果是自身，则无需加入
                        get_firstvt(str[i][j+1],t); //计算该非终结符的firstvt集
                        for(ii=0; ii<t; ii++) { //找到该非终结符对应的产生式
                            if(str[ii][0]==str[i][j+1])
                                break;
                        }
                        for(jj=0; jj<fcnt[ii]; jj++) { //遍历该非终结符的firstvt数组
                            for(tt=0; tt<k; tt++) { //过滤掉已经相同的终结符
                                if(firstvt[i][tt]==firstvt[ii][jj])
                                    break;
                            }
                            if(tt==k) { //如果该终结符还不存在
                                firstvt[i][k++]=firstvt[ii][jj];//添加
                            }
                        }
                    }
                }
            }
        }
        firstvt[i][k]='\0';
        fcnt[i]=k;
        firstflag[i]=1;
    }
}

void output_firstvt(int T) { //输出firstvt集
    for(int i=0; i<T; i++) {
        get_firstvt(str[i][0],T); //输入每个产生式的第一个字符，也就是非终结符；以及产生式的个数
    }
    for(int i=0; i<T; i++) {
        printf("Firstvt[%c]:",str[i][0]);
        for(int j=0; j<fcnt[i]; j++) {
            printf("%c ",firstvt[i][j]);
        }
        puts("");
    }
}
 
void get_lastvt(char s,int t) { //求s非终结符的lastvt值
    int i,j,ii,jj,tt;
    for(i=0; i<t; i++) {
        if(str[i][0]==s)
            break;
    }
    if(!lastflag[i]) {
        int k=lcnt[i];
        for(j=0; str[i][j]!='\0'; j++) {
            if(str[i][j+1]=='|'||str[i][j+1]=='\0') {
                if(is_fin(str[i][j])) {
                    lastvt[i][k++]=str[i][j];
                } else {
                    if(is_fin(str[i][j-1])) {
                        lastvt[i][k++]=str[i][j-1];
                    }
                    if(str[i][j]!=s) {
                        get_lastvt(str[i][j],t);
                        for(ii=0; ii<t; ii++) {
                            if(str[ii][0]==str[i][j])
                                break;
                        }
                        for(jj=0; jj<lcnt[ii]; jj++) {
                            for(tt=0; tt<k; tt++) {
                                if(lastvt[i][tt]==lastvt[ii][jj])
                                    break;
                            }
                            if(tt==k) {
                                lastvt[i][k++]=lastvt[ii][jj];
                            }
                        }
                    }
                }
            }
        }
        lastvt[i][k]='\0';
        lcnt[i]=k;
        lastflag[i]=1;
    }
}
 
void output_lastvt(int T) { //输出lastvt集
    for(int i=0; i<T; i++) {
        get_lastvt(str[i][0],T);
    }
    for(int i=0; i<T; i++) {
        printf("Lastvt[%c]:",str[i][0]);
        for(int j=0; j<lcnt[i]; j++) {
            printf("%c ",lastvt[i][j]);
        }
        puts("");
    }
}
void output(int i,int j,char *str) {
    printf("\t");
    for(int ii=i; ii<=j; ii++)
        printf("%c",str[ii]);
}

int main() {
    int Tnumber;
    while(scanf("%d",&Tnumber)!=EOF) //可以测试输入不同的文法规则集
    {
        NTcount=Tnumber;//xyx 非终结符的数量
        int cnt=0;//终结符的个数
        memset(firstflag,0,sizeof(firstflag));
        memset(lastflag,0,sizeof(lastflag));
        // memset(cmp,0,sizeof(cmp));
        memset(F,0,sizeof(F));
    
        for(int i=0; i<Tnumber; i++) {
            scanf("%s",str[i]);
            fcnt[i]=lcnt[i]=0; //记录每个非终结符的firstvt/lastvt的个数
            NT[i]=str[i][0]; //xyx 记录非终结符数组
        }
        for(int i=0; i<Tnumber; i++) { //记录终结符数组
            for(int j=0; str[i][j]!='\0'; j++) {
                if((str[i][j]<'A'||str[i][j]>'Z')&&(str[i][j]!='-'&&str[i][j]!='>')&&str[i][j]!='|'){
                    T[cnt]=str[i][j];//xyx 记录终结符数组
                    fin[cnt++]=str[i][j];
                }
            }
        }
        T[cnt]='#';
        fin[cnt++]='#';
        Tcount=cnt;//xyx 终结符数量
        fin[cnt]='\0';
        // output_firstvt(T);
        // output_lastvt(T);
        cout<<T<<endl;
        cout<<NT<<endl;
        do_firstvt();
        out_firstvt();
    }
    return 0;
}
/*
3
E->E+T|T
T->T*F|F
F->(E)|a*/