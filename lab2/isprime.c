#include<stdio.h>

int main() {
    int num; // 要检测的整数
    scanf("%d",&num);
    int flag=1;//判断标志

    if(num<=1)
    {
        flag=0;
    }
    else{
        for(int i=2;i*i<=num;i++)
        {
            if(num%i==0)
            {
                flag=0;
                break;
            }
        }
    }

    if(flag==1)
    {
        printf("yes!/n");
    }
    else{
        printf("no!/n");
    }

    return 0;
}
