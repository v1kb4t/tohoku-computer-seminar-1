//Write a mapdata file using array
#include<stdio.h>
#include<stdlib.h>

typedef struct {
        double x,y;
} Position;

typedef struct {
        int id;
        char name[100];
        Position pos;
        double wait;
        int points;
        int next[5];
} Crossing;

void main()
{
	int i=0,j,num=0;
	int conn[100];
	char nexch, choice;
	Crossing intersection[100];
	Position coordinates[100];
	FILE *fp;
        if((fp=fopen("mapdata.dat","w"))==NULL)
        {       printf("Boom, file couldn't be opened\n");
                exit(1);
        }
	

	do
	{
		j=0; conn[i]=0;
		printf("Intersection ID- ");
		scanf(" %d",&intersection[i].id);
	
		printf("Intersection name- ");
		fflush(stdin);
                scanf(" %[^\n]s",intersection[i].name);
		
		printf("Position- ");
		fflush(stdin);
                scanf(" %lf %lf",&coordinates[i].x,&coordinates[i].y);
		
		printf("Waiting time- ");
		fflush(stdin);
		scanf(" %lf",&intersection[i].wait);
		
		printf("No. of Diverging routes- ");
                fflush(stdin);
                scanf(" %d",&intersection[i].points);
	
		if(intersection[i].points<=0)
		{
			printf("This intersection is invalid\n");
		}
		else
		{
			do
			{
                		printf("Neighbouring intersection- ");
               			fflush(stdin);
				scanf(" %d",&intersection[i].next[j]);
				if(j<5)
				{
					printf("Are there anymore neighbouring crossings?\n");
					fflush(stdin);
					scanf(" %c",&nexch);
					j++;
					conn[i]++;
				}
			}while(nexch=='y'&&j<5);
		}
		printf("Do you want to input more data?\n");
		scanf(" %c", &choice);
		i++;
		num++;
	}while(choice=='y');
	fprintf(fp,"num %d",num);
	for(i=0;i<num;i++)
	{
		fprintf(fp,"\n%d\n",intersection[i].id);
		fprintf(fp,"%s\n",intersection[i].name);
		fprintf(fp,"%lf %lf\n",coordinates[i].x,coordinates[i].y);
		fprintf(fp,"%lf\n",intersection[i].wait);
		fprintf(fp,"%d\n",intersection[i].points);
		for(j=0;j<conn[i];j++)
		{
		fprintf(fp,"%d ",intersection[i].next[j]);
		}
	}

	fclose(fp);
}
