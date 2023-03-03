//Write a file using array
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
	Info information[3];
	int i;
	FILE *fp;
        if((fp=fopen("info.dat","w"))==NULL)
        {       printf("Boom, file couldn't be opened\n");
                exit(1);
        }

	for(i=0;i<3;i++)
	{
		printf("Input name> ");
		scanf(" %[^\n]s",information[i].name);
		printf("Age> ");
                scanf("%d",&information[i].age);
		printf("Adress> ");
		fflush(stdin);
                scanf(" %[^\n]s",information[i].address);
		printf("Zipcode> ");
		fflush(stdin);
		scanf("%d",&information[i].zip_code);
	}

	for(i=0;i<3;i++)
        {
                printf("%s, %d, %s, %d\n",information[i].name,information[i].age,information[i].address,information[i].zip_code);
        fprintf(fp,"%s, %d, %s, %d\n",information[i].name,information[i].age,information[i].address,information[i].zip_code);
	}

	fclose(fp);
}
