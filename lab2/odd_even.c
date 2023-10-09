#include<stdio.h>

int main() {
    int num; // 要检测的整数
    scanf("%d",&num);
    int flag=1;//判断标志

    if(num%2==0)
    {
        flag=1;//为偶数
    }
    else
    {
        flag=0;//为奇数
    }
    

    if(flag==1)
    {
        printf("偶数/n");
    }
    else{
        printf("奇数/n");
    }

    return 0;
}
