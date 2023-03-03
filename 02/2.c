//SHM program
#include<stdio.h>
#include<math.h>

double g= 9.8;
double pi  = 3.1416;

int main()
{
	 int n;
       	 double x,y,w;
	 double range = 0.2;
	 double len=0.5;

	 w=sqrt(g/len);
	 x=0.1;

	 for(n=1; n<=10; n++)
	 {
		 y = range + sin(w*x*n);
		 printf("%lf %lf\n",x*n, y);
	 }
return 0;
}
