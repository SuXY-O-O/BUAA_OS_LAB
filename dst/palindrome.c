#include <stdio.h>

int main()
{
	int n=0;
	scanf("%d",&n);
	int a[10];
	int i=0;
	while(n>0)
	{
		a[i]=n%10;
		i++;
		n=n/10;
	}
	int j=0;
	i--;
	int flag=0;
	for(j=0;j<i;j++)
	{
		if(a[j]!=a[i-j])
			flag=1;
	}
	if(flag==1)
		printf("N\n");
	else
		printf("Y\n");
	return 0;
}
