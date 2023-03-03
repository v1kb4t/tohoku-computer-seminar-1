//file writing
#include<stdio.h>
#include<stdlib.h>

void main()
{
	int i;
	char a[100],b[100],c[100];
	FILE *fp;
	if((fp=fopen("profile.dat","w"))==NULL)
	{
		printf("***Boom, file opening failed***\n");
		exit(1);
	}
	printf("Enter your name:- ");
	gets(a);
	fprintf(fp,"Name:- %s\n",a);
	printf("Enter your age:- ");
        gets(b);
	fprintf(fp,"Age:- %s\n",b);
	printf("Enter your place of birth:- ");
        gets(c);
	fprintf(fp,"Place of birth:- %s\n",c);
	
	fclose(fp);
	
}
