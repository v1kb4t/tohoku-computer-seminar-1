//Read data from file
#include<stdio.h>
#include<stdlib.h>

struct info
{
	char name[256];
	int age;
	char address[256];
	int zip_code;
};
typedef struct info Info;

void main()
{
	Info information[100];
	int num;
	int i;
	FILE *fp;
	if((fp=fopen("info.dat","r"))==NULL)
	{	printf("Boom, file couldn't be opened\n");
		exit(1);
	}

	printf("Enter the number of people:- ");
	scanf(" %d",&num);
	if(num>100)
	{
		printf("Can't handle such big data set\n");
		exit(1);
	}
	for(i=0;i<num;i++)
	{
		fscanf(fp, "%[^,]s, %d, %[^,]s, %d\n",
				information[i].name,
				&(information[i].age),
				information[i].address,
				&(information[i].zip_code));
	}
	for (i=0;i<num;i++)
	{
		printf("%s, %d, %s,%d\n",information[i].name,information[i].age,information[i].address,information[i].zip_code);
	}
	fclose(fp);
}
