//user defined function trial
#include<stdio.h>
int kuchbhi(int a, int b)
{
	int z;
	z= a*5 + b*3;
	return z;
}

void main()
{
	int a=1, b=2, c;
	c= kuchbhi(a,b);
	printf("The custom function returned %d\n",c);
	
}
