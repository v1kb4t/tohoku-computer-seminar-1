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
	int i,r=0,j,co=0;
	int match=0;
	for(i=0;i<strlen(s1);i++)
	{
	
		for(j=0;j<strlen(s2);j++)
		{
			if((s1[i]==s2[j])||(s1[i]==s2[j]-32)||(s1[i]==s2[j]+32))
			{	
				co=match;
				match=match+1;
				i+=1;
			}
			else
			{
				co=0;
				match=0;
				i+=1;
				break;
			}
			/*if(match!=co) i+=1;
			else
			{
				i+=1;
				match=0;
				co=0;	
				break;
			}*/
		}
		if(match>=strlen(s2)) break;

	}
	//printf("Match %d\n",match);

	if(match>=strlen(s2)) r=1;
	else r=0;

	return r;
}

void namefind(Crossing intersection[100], char looknm[256],int num)
{
	int i,f=0;
	for(i=0;i<num;i++)
	{
		if(search(intersection[i].name,looknm)!=0)
		{
			printf("Crossing %s | Coordinates %.2lf %.2lf | Distance from station %.2lf\n",intersection[i].name,intersection[i].pos.x,intersection[i].pos.y, dist(intersection[i].pos.x,intersection[i].pos.y));
			f=1;

		}
	}
	if(f==0) printf("Error 404! Intersection not found");

}
void idfind(Crossing intersection[100], int lookid, int num)
{
	int i,f=0;
	for(i=0;i<num;i++)
	{
		if(intersection[i].id==lookid)
		{
			printf("Crossing %s | Coordinates %.2lf %.2lf | Distance from station %.2lf\n",intersection[i].name,intersection[i].pos.x,intersection[i].pos.y, dist(intersection[i].pos.x,intersection[i].pos.y));
			f=1;

		}
	}
	if(f==0) printf("Error 404! Intersection not found");

}
void posfind(Crossing intersection[100], double lookpos[2], int num)
{
	int i;
	int ref=0;
	double r,minr;
	double a=lookpos[0], b=lookpos[1];
	for(i=0;i<num;i++)
	{
		minr=dist(intersection[ref].pos.x-a,intersection[ref].pos.y-b);
		r=dist(intersection[i].pos.x-a,intersection[i].pos.y-b);
		if(r<minr)
			ref=i;
	}
	printf("Crossing %s | Coordinates %.2lf %.2lf | Distance from desired coordinates %.2lf\n",intersection[ref].name,intersection[ref].pos.x,intersection[ref].pos.y,minr);
}
void main()
{
	int i,j,num,crit;
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
		fscanf(fp,"%d,%lf,%lf,%d,%[^,],%d",&intersection[i].id,&intersection[i].pos.x,&intersection[i].pos.y,&intersection[i].wait,intersection[i].name,&intersection[i].points);

		

		for(j=0;j<intersection[i].points;j++)
		{
			fscanf(fp,",%d",&intersection[i].next[j]);
		}
		fscanf(fp,"\n");	
	}



	char looknm[256];
	int lookid;
	double lookpos[2];
	int f=0;
	printf("Select your search parameters->\n1) Name\n2) ID\n3) Coordinates\n=>");
	scanf(" %d",&crit);
	if(crit==1)
	{
		printf("Input the name of the intersection(alphabets only)-> ");
		scanf(" %[^\n]",looknm);
		namefind(intersection,looknm,num);
	}

	if(crit==2)
	{
		printf("Input the id of the intersection(integers only)-> ");
		scanf(" %d",&lookid);
		idfind(intersection,lookid,num);

	}

	if(crit==3)
	{
		printf("Input the coordinates of the intersection with a space in between(real numbers only)-> ");
		scanf(" %lf %lf",&lookpos[0],&lookpos[1]);
		posfind(intersection,lookpos,num);

	}


	fclose(fp);
	}

