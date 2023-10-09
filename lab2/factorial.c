#include <stdio.h>

int compute(int i,int n)
{
    int f=1;
     while (i <= n)
    {
        f = f * i;
        i = i + 1;
    }
    return f;
}

int main()
{
    
    int i, n, f;
    scanf("%d", &n); 

    i = 2;
    f = 1;

   f=compute(i,n);

    printf("%d", f); 

    return 0;
}

