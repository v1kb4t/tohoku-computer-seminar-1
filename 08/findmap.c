/*load map data, search locations, display in opengl*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <unistd.h>
#include <GL/glfw.h>
#include<FTGL/ftgl.h>

/* Macro definition to be added */


#define Radius_Marker 0.2 /* The radius of a marker */

/* Definition of coordinate conversion macro */
#define ORIGIN_X 1.0
#define ORIGIN_Y -1.0
#define REAL_SIZE_X 18.0
#define REAL_SIZE_Y 18.0

#ifndef FONT_FILENAME
#define FONT_FILENAME "/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf"
#endif
FTGLfont *font;

void outtextxy(double x, double y, char const *t)
{
	double const scale = 1.0;
	glPushMatrix();
	glTranslated(x,y, 0.0);
	glScaled(scale,scale,scale);
	ftglRenderFont(font,t,FTGL_RENDER_ALL);
	glPopMatrix();
}

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

void setupfont()
{
	font=ftglCreateExtrudeFont(FONT_FILENAME);
	if(font==NULL)
	{
		perror(FONT_FILENAME);
		fprintf(stderr,"couldn't load ");
		exit(1);

	}
	ftglSetFontFaceSize(font,12,12);
	ftglSetFontDepth(font,0.01);
	ftglSetFontOutset(font,0,0.1);
	ftglSetFontCharMap(font,ft_encoding_unicode);

}

int search_nearest(Crossing cross[100],int id, int goal) {
    int i;
    double min_distance = 1e100; /* Initialize with a large numerical value temporarily */
    double distance;

    for (i = 0; i < cross[id].points; i++)
    {
        distance = hypot((cross[ cross[id].next[i] ].pos.x - cross[goal].pos.x),(cross[cross[id].next[i]].pos.y - cross[goal].pos.y));

        if(distance <= min_distance){
            min_distance = distance;
            nearest = cross[id].next[i];
        }
    }
    return nearest;
}
/*function for plotting the intersections and roads*/
void opengl(Crossing cross[100],int start,int mark,int num)
{
	 int crossing_number; /* The number of crossings */
    int vehicle_pathIterator; /* The position of the moving object on the path (it represents
                                 that this is the n-th road) */

    int vehicle_stepOnEdge; /* The position of the moving object on the road (it represents that this is the n-th step taken on this road) */
    double vehicle_x =500.0, vehicle_y = 600.0; /* The coordinate of the moving object */
 int vehicle_goal; /* The final destination point of the moving object */
    int vehicle_edgeFrom; /* The position of the moving object on the path 
                             (it represents that this is the n-th road) */
    int vehicle_edgeTo; /* The position of the moving object on the path 
                           (it represents that this is the n-th road) */
    
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
	setupfont();
	

	 vehicle_pathIterator=0;
    vehicle_stepOnEdge=0;
#if 1
    vehicle_edgeFrom = start; /* From starting point */
    vehicle_goal = mark; /* To destination */ 
#else 
    vehicle_edgeFrom = start; /* From Izumi Ohashi */ 
    vehicle_goal = 71; /* To Yagiyama Zoological Park */
#endif
    vehicle_edgeTo = vehicle_edgeFrom;
    vehicle_stepOnEdge = 0;
   
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
                        circle(cross[i].pos.x*40+800,cross[i].pos.y*40+500,3);

                        for(j=0;j<cross[i].points;j++)   //loop for constructing the roads
                        {
                                glColor3d(0.0,1.0,1.0);
                                line(cross[i].pos.x*40+800,cross[i].pos.y*40+500,cross[cross[i].next[j]].pos.x*40+800,cross[cross[i].next[j]].pos.y*40+500);
                        }

                }
                glColor3f(0.0,1.0,0.0); //marks the start point
                marker(cross[start].pos.x*40+800,cross[start].pos.y*40+500,4);
		                     glColor3d(1.0,.7,1.0);
                        outtextxy(cross[start].pos.x*40+800,cross[start].pos.y*40+500,cross[start].name);

			 glColor3f(1.0,0.0,0.0); //marks the final destination point
                marker(cross[mark].pos.x*40+800,cross[mark].pos.y*40+500,4);
                                     glColor3d(1.0,.5,1.0);
                        outtextxy(cross[mark].pos.x*40+800,cross[mark].pos.y*40+500,cross[mark].name);
/* Move the moving object forward and calculate the coordinate */
        if (vehicle_edgeFrom != vehicle_goal) {
            /* Since the goal has not been reached, move forward the position of the moving object */
            

            vehicle_edgeTo = search_nearest(cross,vehicle_edgeFrom, vehicle_goal);
	    
	
            /* Set (x0, y0), (x1, y1) for the current and next crossings */
            double x0 = cross[vehicle_edgeFrom].pos.x*40+800; /* The last crossing passed, x */
            double y0 = cross[vehicle_edgeFrom].pos.y*40+500; /* The last crossing passed, y */
            double x1 = cross[vehicle_edgeTo].pos.x*40+800; /* The next crossing to pass, x */
            double y1 = cross[vehicle_edgeTo].pos.y*40+500; /* The next crossing to pass, y */
	    glColor3d(1.0,0.0,1.0);
            line(x0,y0,x1,y1);
	    glColor3d(1.0,.7,1.0);
            outtextxy(x1,y1,cross[vehicle_edgeTo].name);




            double distance = hypot(x1 - x0, y1 - y0); /* Direct distance */
            int steps = (int)(distance / 0.1);

            /* Move forward on the road and update the coordinate */
            vehicle_stepOnEdge++;

            vehicle_x = x0 + (x1 - x0) / steps * vehicle_stepOnEdge; 
            vehicle_y = y0 + (y1 - y0) / steps * vehicle_stepOnEdge;

            if (vehicle_stepOnEdge >= steps) { /* After reaching the crossing, enter the next road */
                vehicle_edgeFrom = vehicle_edgeTo; /* Increment the index that represents the current crossing on the path */
                vehicle_stepOnEdge = 0; /* Reset the index that represents that it is the n-th step on the edge */
           }

        }

        /* Display the moving object at the current position */
        glColor3d(1.0, 1.0, 1.0);
        marker(vehicle_x, vehicle_y, 5.0);

        glfwSwapBuffers(); /* Swap the front buffer with the back buffer */ 
        usleep(1 * 1000); /* Wait for about 1 millisecond */
    }

              

        glfwTerminate();

}
void main()
{
	int i,j,num,crit;
	/*calling the structures*/
	Crossing intersection[100];
	Position coordinates[100];
	int start,mark;

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
		printf("Input the name of the starting intersection(alphabets only)-> ");
		scanf(" %[^\n]",looknm);
		start=namefind(intersection,looknm,num);

		printf("Input the name of the destination intersection(alphabets only)-> ");
                scanf(" %[^\n]",looknm);
                mark=namefind(intersection,looknm,num);

	}

	/*id search*/
	if(crit==2)
	{
		printf("Input the id of the starting intersection(integers only)-> ");
		scanf(" %d",&lookid);
		start=idfind(intersection,lookid,num);

		printf("Input the id of the destination intersection(integers only)-> ");
                scanf(" %d",&lookid);
                mark=idfind(intersection,lookid,num);


	}

	/*coordinates serach*/
	if(crit==3)
	{
		printf("Input the coordinates of the starting intersection with a space in between(real numbers only)-> ");
		scanf(" %lf %lf",&lookpos[0],&lookpos[1]);
		start=posfind(intersection,lookpos,num);

		printf("Input the coordinates of the destination intersection with a space in between(real numbers only)-> ");
                scanf(" %lf %lf",&lookpos[0],&lookpos[1]);
                mark=posfind(intersection,lookpos,num);


	}
	

	opengl(intersection,start,mark,num);	//calling function to plot the intersections and roads
	
	fclose(fp);
}

