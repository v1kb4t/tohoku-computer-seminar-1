#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

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

double dist(double x,double y)
{
	double r;
	r=sqrt(x*x+y*y);
	return r;
}

int search(char s1[100], char s2[100])
{
	int i,r;
	int match=0;
	for(i=0;i<strlen(s2);i++)
	{
		if((s1[i]==s2[i])||(s1[i]==s2[i]-32)||(s1[i]==s2[i]+32))
		{
			match=match+1;
		}

	}
	//printf("match %d\n",match);
	//if(strlen(s1)==strlen(s2))
	//{
	//	if(match==strlen(s2)) r=1;
	//	else r=0;
	//}
	//if((s1[0]==s2[0])||(s1[0]==s2[0]-32))
	//{
	//	if(match>2) r=1;
	//	else r=0;
	//}
	if(match==strlen(s2)) r=1;
            else r=0;

	return r;
}
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


	char look[256];
	int f=0;
	printf("Input the name of the intersection(alphabets only)-> ");
	scanf("%[^\n]",look);
	for(i=0;i<num;i++)
	{
		if(search(intersection[i].name,look)!=0)
		{
			printf("Crossing %s|Coordinates %.2lf %.2lf| Distance from station %.2lf\n",intersection[i].name,coordinates[i].x,coordinates[i].y, dist(coordinates[i].x,coordinates[i].y));
			f=1;
	
		}
	}
	if(f==0) printf("Error 404! Intersection not found");
}
