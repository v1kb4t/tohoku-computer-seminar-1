//integration as a limit of sum
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double func(double x)
{
	return x*x;
}
void main()
{
	double a, b,m,i;
	FILE *fp;
	if((fp=fopen("y.dat","w"))==NULL)
	{
		printf("File couldn't open\n");
	}
	printf("Input the upper and lower limits followed by a space:- ");
	scanf("%lf %lf",&b, &a);
	printf("Enter the number of intervals:- ");
	scanf("%lf",&m);
	i=(b-a)/m;
	double x,y,z=0,n;
	for(x=a;x<=b;x+=i)
	{
		y=func(x);
		z=z+y*i;
		fprintf(fp,"%lf %lf\n",x,y);
	}
	n=(pow(b,3)-pow(a,3))/3;
	printf("Numerical result of the integral is %lf\n",n);
	printf("Integration as a limit of sum is %lf\n",z);
	fclose(fp);

	
	 
}
