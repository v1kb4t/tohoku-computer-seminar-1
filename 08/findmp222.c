/*load map data, search locations, display in opengl*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <unistd.h>
#include <GL/glfw.h>

/*structure for storing coordinate values*/
typedef struct {
	double x,y;
} Position;

/*structure for loading the map data*/
typedef struct {
	int id;
	char name[100];
	Position pos;
	int wait;
	int points;
	int next[5];
} Crossing;

void font()
{
        font=ftglCreateExtrude(FONT_FILENAME);
        if(font==NULL)
        {
                perror(FONT_FILENAME);
                fprintf(stderr,"couldn't load ")
        }

}

/*function for drawing st. lines*/
void line(double x0, double y0, double x1, double y1)
{
	glBegin(GL_LINES);
	glVertex2d(x0,y0);
	glVertex2d(x1,y1);
	glEnd();
}

/*function for drawing circles*/
void circle(double x, double y, double r)
{
	int const N = 24; /* Draw a circle using line segments by dividing the circumference into 24 pieces */
	int i;

	glBegin(GL_LINE_LOOP);
	for (i=0;i<N;i++)
		glVertex2d(x+cos(2*M_PI*i/N)*r, y+sin(2*M_PI*i/N)*r);
	glEnd();
}

/*function for drawing a solid circle*/
void marker(double x, double y,double r)
{
	int i;
	double pi=3.14;
	glBegin(GL_TRIANGLE_FAN);

	glVertex2f(x, y); //center of triangles


	for(i = 0; i <= 24;i++) {

		glVertex2f(x+r*cos(i*2*pi / 24),y+r*sin(i*2*pi / 24));
	}
	glEnd();
}

/*function for calculating the distance of a point from another point*/
double dist(double x,double y)
{
	double r;
	r=sqrt(x*x+y*y);
	return r;
}

/*my own custom search function for searching a place using name parameter. it is case insensitive and searches out for a string of characters. works like a search function in notepad. not advanced enough to predict from a wrong keyword*/
int search(char s1[100], char s2[100])
{
	int i,r=0,j,co=0;
	int match=0;
	for(i=0;i<strlen(s1);i++)	//loop for compairing existing name data
	{

		for(j=0;j<strlen(s2);j++)	//loop for matching the entered keyword with existing name data
		{
			if((s1[i]==s2[j])||(s1[i]==s2[j]-32)||(s1[i]==s2[j]+32))	//case insensitive search using the difference in ascii values of capital and small letters
			{	
				co=match;	//used this to compare the continuity of character matching
				match=match+1;	//advances the match counter
				i+=1;	//advances the  position for checking match
			}
			else
			{
				co=0;	//resets for discontinuity of character matching
				match=0;	
				break;	//advances the loop for checking next character
			}

		}
		if(match>=strlen(s2)) break;	//exits the loop if all the entered characters are matched

	}


	if(match>=strlen(s2)) r=1;	//all the characters are matched
	else r=0;	//no match found

	return r;
}

/*function for searching an intersection using name parameter*/
int namefind(Crossing intersection[100], char looknm[256],int num)
{
	int mark;
	int i,f=0,count=0,j=0,final=1;
	for(i=0;i<num;i++)	//compairing the entered name with the exising name data
	{
		if(search(intersection[i].name,looknm)!=0)	//match affirmative
		{
			f=1;	//variable for affirmative match
			count+=1; 	//counter for total no. of matches found
		}

	}

	int s[count];
	if(count>1)	//greater than 1 match condition
	{
		if(f==1) printf("\nDid you mean:\n");
		while(j<count)	//displays all the matches
		{
			for(i=0;i<num;i++)
			{

				if(search(intersection[i].name,looknm)!=0)
				{
					printf("%d> Crossing %s | Coordinates %.2lf %.2lf | Distance from station %.2lf\n",j+1,intersection[i].name,intersection[i].pos.x,intersection[i].pos.y, dist(intersection[i].pos.x,intersection[i].pos.y));
					s[j]=i;		//stores the id of matched intersection
					j+=1;	//advances the position of storing match ids

				}
			}
		}
		printf("\nSelect the specific intersection> ");	//asks for 1 particular intersection
		scanf(" %d",&final);	//takes user input for displaying final match
	}
	if(count==1) 	//condition for exactly 1 match 
	{
		for(i=0;i<num;i++)	//loop for finding the match
		{

			if(search(intersection[i].name,looknm)!=0)	//match affirmative
			{
				printf("Crossing %s | Coordinates %.2lf %.2lf | Distance from station %.2lf\n",intersection[i].name,intersection[i].pos.x,intersection[i].pos.y, dist(intersection[i].pos.x,intersection[i].pos.y));
				s[0]=i;	//stores id of the matched intersection
			}

		}
	}
	mark=s[final-1];	//stores the id of the final match
	printf("\nYour final destination is:\n");
	printf("Crossing %s | Coordinates %.2lf %.2lf | Distance from station %.2lf\n",intersection[s[final-1]].name,intersection[s[final-1]].pos.x,intersection[s[final-1]].pos.y, dist(intersection[s[final-1]].pos.x,intersection[s[final-1]].pos.y));

	int ref=0;
        double r,minr;
        for(i=0;i<num;i++)      //loop for checking all the data points for minimum distance
        {
		if(i!=s[final-1])	//condition for skipping self checking
		{	
                minr=dist(intersection[ref].pos.x-intersection[s[final-1]].pos.x,intersection[ref].pos.y-intersection[s[final-1]].pos.y); //calculating minimum distance between data point from the final point
                r=dist(intersection[i].pos.x-intersection[s[final-1]].pos.x,intersection[i].pos.y-intersection[s[final-1]].pos.y);        //calculating distance between each data point from final point
                if(r<minr)      //setting new reference based on minimum distance value
                        ref=i;
		}
        }
	printf("Closest intersection %s | Distance from destination %.2lf\n",intersection[ref].name, minr);

	if(f==0) 	//no match condition
	{
		printf("Error 404! Intersection not found\n");
		mark=0;
	}
	return mark;
}

/*function for searching using id*/
int idfind(Crossing intersection[100], int lookid, int num)
{
	int i,f=0, mark;
	for(i=0;i<num;i++)
	{
		if(intersection[i].id==lookid)	//match affirmative
		{
			printf("Crossing %s | Coordinates %.2lf %.2lf | Distance from station %.2lf\n",intersection[i].name,intersection[i].pos.x,intersection[i].pos.y, dist(intersection[i].pos.x,intersection[i].pos.y));
			f=1;
			mark=i;	//stores the value of the final match

		}
	}
	if(f==0) //no match condition
	{
		printf("Error 404! Intersection not found");
		mark=0;
	}
	return mark;

}

/*function for searching using coordinates*/
int posfind(Crossing intersection[100], double lookpos[2], int num)
{
	int i,mark;
	int ref=0;
	double r,minr;
	double a=lookpos[0], b=lookpos[1];
	for(i=0;i<num;i++)	//loop for checking all the data points
	{
		minr=dist(intersection[ref].pos.x-a,intersection[ref].pos.y-b);	//calculating minimum distance between data point from the entered values
		r=dist(intersection[i].pos.x-a,intersection[i].pos.y-b);	//calculating distance between each data point from enterd values
		if(r<minr)	//setting new reference based on minimum distance value
			ref=i;
	}
	printf("Crossing %s | Coordinates %.2lf %.2lf | Distance from desired coordinates %.2lf\n",intersection[ref].name,intersection[ref].pos.x,intersection[ref].pos.y,minr);
	mark=ref;	//stores value of minimum distance point
	return mark;
}

/*function for plotting the intersections and roads*/
void opengl(Crossing intersection[100],int mark,int num)
{
	int i,j;
       	int width = 1600, height = 900; /* Window size */

        /* Initialize the graphic environment and open a window */
        glfwInit();
        glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

        /* Set up a conversion matrix for projection */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,width,0,height,-1.0,1.0); /* Project the space with this range to a viewport */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); /* Do not convert coordinates except for projection purposes */


        for(;;){



                /* The program is ended if the user pushes Esc or closes the window */
                if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
                        break;

                glfwGetWindowSize(&width,&height); /* Obtain the current window size */
                glViewport(0,0,width, height); /* Use the whole window as a viewport) */



                glClearColor(0.0f,0.0f,0.0f,0.0f);
                glClear(GL_COLOR_BUFFER_BIT); /* Paint the back buffer in black */

                /* Draw different figures on the back buffer */

                /*plots all the intersections and draws lines according to connection data*/
                for(i=0;i<num;i++){
                        glColor3f(1.0,1.0,0.0);
                        circle(intersection[i].pos.x*40+800,intersection[i].pos.y*40+500,3);
                        for(j=0;j<intersection[i].points;j++)   //loop for constructing the loops
                        {
                                glColor3d(0.0,1.0,1.0);
                                line(intersection[i].pos.x*40+800,intersection[i].pos.y*40+500,intersection[intersection[i].next[j]].pos.x*40+800,intersection[intersection[i].next[j]].pos.y*40+500);
                        }

                }
                glColor3f(1.0,0.0,0.0); //marks the final destination point
                marker(intersection[mark].pos.x*40+800,intersection[mark].pos.y*40+500,4);

                /* Switch between the front and back buffers */
                glfwSwapBuffers();
                /* This has made images drawn so far visible */

        }

        glfwTerminate();

}
void main()
{
	int i,j,num,crit;
	/*calling the structures*/
	Crossing intersection[100];
	Position coordinates[100];
	int mark;

	/*file opening and use*/
	FILE *fp;
	if((fp=fopen("map.dat","r"))==NULL)
	{       printf("Boom, file couldn't be opened\n");
		exit(1);
	}



	/*loading data from map.dat file*/
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
	printf("Select your search parameters->\n1) Name\n2) ID\n3) Coordinates\n=>");	//giving user options for selecting search parameters
	scanf(" %d",&crit);	//user inputs choice of parameters

	/*name search*/
	if(crit==1)
	{
		printf("Input the name of the intersection(alphabets only)-> ");
		scanf(" %[^\n]",looknm);
		mark=namefind(intersection,looknm,num);
	}

	/*id search*/
	if(crit==2)
	{
		printf("Input the id of the intersection(integers only)-> ");
		scanf(" %d",&lookid);
		mark=idfind(intersection,lookid,num);

	}

	/*coordinates serach*/
	if(crit==3)
	{
		printf("Input the coordinates of the intersection with a space in between(real numbers only)-> ");
		scanf(" %lf %lf",&lookpos[0],&lookpos[1]);
		mark=posfind(intersection,lookpos,num);

	}
	
	opengl(intersection,mark,num);	//calling function to plot the intersections and roads
	
	fclose(fp);
}

