#include <stdio.h>

#define MAX 100

#if MAX == 100 
#define MIN 10
#endif


int main() {
	// jie cheng
	int i, n, f;
	printf("请输入一个整数：");
	scanf("%d", &n);

	i = 2;
	f = 1;

	if(n>MAX)
	{
		return 0;
	}
	while (i <= n) {
		f = f * i;
		i = i + 1;
	}

	printf("%d的阶乘是%d\n", n, f);
	return 0;
}
