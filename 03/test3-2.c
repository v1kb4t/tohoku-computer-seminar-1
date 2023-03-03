//printing file contents in terminal
#include<stdio.h>
#include<stdlib.h>

void main()
{
	char firstname[100],lastname[100], age[100], place[100];
	FILE *fp;
	if((fp=fopen("profile.dat","r"))==NULL)
	{
		printf("Boom...file not open\n");
	}
	fscanf(fp,"Name:- %s %s\nAge:- %s\nPlace of birth:- %s", firstname,lastname, age, place);
	printf("Name:- %s %s\nAge:- %s\nPlace of birth:- %s\n",firstname,lastname, age, place);
	fclose(fp);
}
