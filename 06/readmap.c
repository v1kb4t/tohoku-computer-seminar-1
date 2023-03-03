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
        double wait;
        int points;
        char next[10];
} Crossing;

void main()
{
        int i=0,j,num;
        Crossing intersection[100];
        Position coordinates[100];
	FILE *fp;
        if((fp=fopen("mapdata.dat","r"))==NULL)
        {       printf("Boom, file couldn't be opened\n");
                exit(1);
        }

	fscanf(fp,"num %d\n",&num);
	while(i<num)
	{
		fscanf(fp,"%d\n",&intersection[i].id);
		
		fscanf(fp,"%[^\n]s",intersection[i].name);

		fscanf(fp,"%lf %lf\n",&coordinates[i].x,&coordinates[i].y);

		fscanf(fp,"%lf\n",&intersection[i].wait);

		fscanf(fp,"%d\n",&intersection[i].points);
		
                fscanf(fp,"%[^\n]s ",intersection[i].next);
		
		i++;

	}

	printf("%d",num);
	for(i=0;i<num;i++)
        {
                printf("\n%d\n",intersection[i].id);
                printf("%s\n",intersection[i].name);
                printf("%lf %lf\n",coordinates[i].x,coordinates[i].y);
                printf("%lf\n",intersection[i].wait);
                printf("%d\n",intersection[i].points);
                printf("%s ",intersection[i].next);
                
        }

	fclose(fp);
}


