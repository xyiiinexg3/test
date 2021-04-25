#include <stdio.h>
#include <string.h>

char InputString[255]={0};
int ptr=0;
int flag=0;

int E1();
int E2();
int T1();
int T2();
int F1();
int F2();
int P();
int forward();
int error();

int E1(){
    printf("E->TE'\n");
    if(T1()==0){
        if(T2()==0){
            return 0;
        }
    }else{
        return -1;
    }
}
int E2(){
    //由终结符开头
    if(InputString[ptr]=='+'){
        printf("E'->+E ----------+\n");
        forward();
        if(E1()==0){
            return 0;
        }
    }
    //ε follow（E'）
    else if(InputString[ptr]=='#'||InputString[ptr]==')'){
        printf("E'->ε \n");
        return 0;
    }else{
        return -1;
    }
}
int T1(){
    printf("T->FT'\n");
    if(F1()==0){
        if(T2()==0){
            return 0;
        }
    }else{
        return -1;
    }
}
int T2(){
    //ε follow(T')
    if(InputString[ptr]=='#'||InputString[ptr]==')'||InputString[ptr]=='+'){
        printf("T'->ε \n");
        return 0;
    }
    //由非终结符开头 first(T)
    else if(InputString[ptr]=='('||InputString[ptr]=='a'||InputString[ptr]=='b'||InputString[ptr]=='^'){
        printf("T'->T\n");
        if(T1()==0){
            return 0;
        }
    }
    else{
        return -1;
    }
}
int F1(){
    printf("F->PF'\n");
    P();
    F2();
}
int F2(){
    //由终结符开头
    if(InputString[ptr]=='*'){
        printf("F'->*F' ----------*\n");
        forward();
        F2();
    }
    //ε follow(F')
    else if(InputString[ptr]=='c'||InputString[ptr]=='a'||InputString[ptr]=='b'||InputString[ptr]=='#'||InputString[ptr]==')'||InputString[ptr]=='+'){
        printf("F'->ε \n");
        return ;
    }
    else{
        error();
    }
}
int P(){
    //由终结符开头
    if(InputString[ptr]=='('){
        printf("P->(E) ----------(\n");
        forward();
        E1();
        if(InputString[ptr]==')'){
            printf("P->(E) ----------)\n");
            forward();
            return ;
        }else{
            error();
        }
    }else if(InputString[ptr]=='a'||InputString[ptr]=='b'||InputString[ptr]=='^'){
        printf("P->%c ----------%c\n",InputString[ptr],InputString[ptr]);
        forward();
        return ;
    }else {
        error();
    }
}
int forward(){
    ptr++;
}
int error(){
    flag++;//对于错误处理，不能在递归中直接退出
    
}
int main(){
    printf("please input the tested string:\n");
    scanf("%s",&InputString);
    printf("the string is %s\n",InputString);
    E1();
    if(flag==0){
        printf("right!\n");
    }else{
        printf("wrong!\n");
    }
    getchar();
    return 0;
}
