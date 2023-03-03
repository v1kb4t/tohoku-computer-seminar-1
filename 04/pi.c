#include<stdio.h>
#include<stdlib.h>
#include<math.h>

long double arctan(long double x, int n)
{
	int i;
	long double y=0.0;
	for(i=1;i<=n;i++)
	{
		y=y+pow(-1,i+1)*pow(x,2*i-1)/(2*i-1);
	}
	return y;
}

void main()
{
	int n;
	long double x, i;
	long double pie=0.0;
	printf("Enter the value of n:- ");
	scanf(" %d",&n);
	pie= 16*arctan(1.0/5,n)-4*arctan(1.0/239,n);
	printf("pi[%d]=%1.1111Lf\n",n,pie);
}
