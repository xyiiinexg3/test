#include <stdio.h>
#include <string.h>

int num1;
int num2;
char inp[250];
char op;
int res;
int flag;

void input(){
    //初始化
    flag=0;
    memset(inp,0,250);
    num1=0;
    num2=0;
    res=0;
    op=0;
    //输入提示
    printf("please input:\n");
    gets(inp);
    //数据处理
    int i=0;//inp
    while(inp[i]>='0'&&inp[i]<'9'){//get num1
        num1 = num1*10 + (inp[i]-'0');
        i++;
    }
    printf("num1=%d\n",num1);
    op=inp[i];//get op
    i++;
    while(inp[i]>='0'&&inp[i]<'9'){//get num2
        num2 = num2*10 + (inp[i]-'0');
        i++;
    } 
    printf("num2=%d\n",num2);
    //计算结果
    switch (op)
    {
    case '+':
        res=num1+num2;
        break;
    case '-':
        res=num1-num2;
        break;
    case '*':
        res=num1*num2;
        break;
    case '/':
        res=num1/num2;
        break;
    default:
        flag=1;
        break;
    }
    if(flag==1){
        printf("the input is invalid!\n");
    }else{
        printf("%s %d\n",inp,res);
    }

}

int main(){
    while(1){
        input();
    }
}