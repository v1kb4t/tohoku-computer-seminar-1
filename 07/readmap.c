//Read map data
#include<stdio.h>
#include<stdlib.h>

typedef struct {
        double x,y;
} Position;

typedef struct {
        int id;
        char name[100];
        Position pos;
        int wait;
        int points;
        int next[5];
} Crossing;

void main()
{
        int i,j,num;
        Crossing intersection[100];
        Position coordinates[100];
	FILE *fp;
        if((fp=fopen("map.dat","r"))==NULL)
        {       printf("Boom, file couldn't be opened\n");
                exit(1);
        }

	fscanf(fp,"%d\n",&num);
	for(i=0;i<num;i++)
	{
		fscanf(fp,"%d,%lf,%lf,%d,%[^,],%d",&intersection[i].id,&coordinates[i].x,&coordinates[i].y,&intersection[i].wait,intersection[i].name,&intersection[i].points);
	
		for(j=0;j<intersection[i].points;j++)
		{
		fscanf(fp,",%d",&intersection[i].next[j]);
		}
		fscanf(fp,"\n");

	}

	printf("%d\n",num);
	for(i=0;i<num;i++)
        {
                printf("%d,%lf,%lf,%d,%s,%d",intersection[i].id,coordinates[i].x,coordinates[i].y,intersection[i].wait,intersection[i].name,intersection[i].points);
                for(j=0;j<intersection[i].points;j++)
		{
                	printf(",%d",intersection[i].next[j]);
		}
		printf("\n");
                
        }

	fclose(fp);
}


