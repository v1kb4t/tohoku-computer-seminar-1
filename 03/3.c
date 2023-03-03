//file operations
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void main()
{
	int n;
	double x, y,z;
	FILE *fp;
	if((fp=fopen("result.dat","w"))==NULL)
	{
		printf("***Bazinga, File opened***\n");
		exit(1);
	}
	for(n=1;n<=10;n++)
	{
		x=n;
		y=log(x);
		z=sqrt(x);
		fprintf(fp, "n=%d, x=%lf, y= %lf, z=%lf\n",n, x, y,z);
	}
	fclose(fp);
}
