//quadratic equation
#include<stdio.h>
#include<math.h>

double *roots(double a, double b, double c)
{
	static double x[2];
	double d;
	d=(b*b-4*a*c);
	if(d>=0)
	{
		x[0]=(-b+sqrt(d))/(2*a);
		x[1]=(-b-sqrt(d))/(2*a);
	}
	else
	{
		printf("x1= %lf+i*%lf\n",-b/(2*a), sqrt(-d)/(2*a));
		printf("x1= %lf-i*%lf\n",-b/(2*a), sqrt(-d)/(2*a));

	}
	return x;
}

int main()
{
	double a,b,c;
	int i=0;
	double *p;
	printf("Enter the coefficients of the quadratic equation (a,b,c):- ");
	scanf("%lf %lf %lf", &a, &b, &c);
	p=roots(a,b,c);
	for(i=0;i<2;i++)
	{
		printf("real roots are: x%d = %lf\n",i,*(p+i));
	}
	return 0;


}
