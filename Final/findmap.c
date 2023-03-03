/*load map data, search locations, display in opengl*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <unistd.h>
#include <GL/glfw.h>
#include<FTGL/ftgl.h>


#ifndef FONT_FILENAME
#define FONT_FILENAME "./roboto/Roboto-Condensed.ttf"
#endif
FTGLfont *font;

//Defining control keys, origin and size of map
#define GLFW_KEY_Z   90
#define GLFW_KEY_X   88
#define GLFW_KEY_D   68
#define GLFW_KEY_A   65
#define GLFW_KEY_S   83
#define GLFW_KEY_W   87
#define GLFW_KEY_Q   81
#define GLFW_KEY_E   69
#define GLFW_KEY_R   82
#define GLFW_KEY_T   84

//text output function
void outtextxy(double x, double y, char const *t,double ang,double scale)
{
	glPushMatrix();
	glTranslated(x,y, 0.0);
	glRotatef(-ang,0,0,1);
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
#define PathNumber 100 /* The maximum number of crossings on a path */

int path[PathNumber + 1]; /* Path: the crossing IDs are stored in the order which they will be passed. */

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

//drwaing the car like an arrow
void arrow(double x, double y, double x1, double y1)
{
	glLineWidth(2.0);


	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x-4*x1+y1,y-4*y1-x1);
	glVertex2f(x-3*x1,y-3*y1);
	glVertex2f(x-4*x1-y1,y-4*y1+x1);

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
		printf("\nSelect the index of specific intersection> ");	//asks for 1 particular intersection
		scanf(" %d",&final);	//takes user input for displaying final match
	}
	if(count==1) 	//condition for exactly 1 match 
	{
		for(i=0;i<num;i++)	//loop for finding the match
		{

			if(search(intersection[i].name,looknm)!=0)	//match affirmative
			{
				printf("ID %d | Crossing %s | Coordinates %.2lf %.2lf |\n",intersection[i].id,intersection[i].name,intersection[i].pos.x,intersection[i].pos.y);
				s[0]=i;	//stores id of the matched intersection
			}

		}
	}
	mark=s[final-1];	//stores the id of the final match
	printf("\nYour final destination is:\n");
	printf("%d |  %s | %.2lf %.2lf\n",intersection[mark].id,intersection[mark].name,intersection[mark].pos.x,intersection[mark].pos.y);

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
			printf("\n%d |  %s | %.2lf %.2lf\n",intersection[i].id,intersection[i].name,intersection[i].pos.x,intersection[i].pos.y);
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
	printf("\n%d |  %s | %.2lf %.2lf\n",intersection[ref].id,intersection[ref].name,intersection[ref].pos.x,intersection[ref].pos.y);
	mark=ref;	//stores value of minimum distance point
	return mark;
}

//function for setting up the font
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

//array for saving the djikstra info
typedef struct{
	int id;
	int previous;
	double distance;
	int status;
}djikarr;	

//djikstra algorithm for path calculation
int djikstra(Crossing intersection[100], djikarr djik[100],int start, int goal, int num)
{
	int i,j,k,ref;
	double p,minr;
	for(i=0;i<num;i++)
	{
		djik[i].distance=10000;
		djik[i].status=0;
		djik[i].previous=-1;
	}
	djik[start].id=start;
	djik[start].distance=0;



	k=start;
	do
	{
		djik[k].status=1;
		for(j=0;j<intersection[k].points;j++)      //loop for checking all the data points
		{	
			djik[intersection[k].next[j]].id=intersection[k].next[j];
			p=dist(intersection[k].pos.x-intersection[intersection[k].next[j]].pos.x,intersection[k].pos.y-intersection[intersection[k].next[j]].pos.y);
			if(djik[intersection[k].next[j]].distance>djik[k].distance+p)
			{
				djik[intersection[k].next[j]].distance=djik[k].distance+p; //calculating minimum distance between data point from the entered values
				djik[intersection[k].next[j]].previous=k;
			}


		}
		minr=10000;
		for(i=0;i<num;i++)
		{
			for(j=0;j<intersection[i].points;j++)
			{
				if((minr>djik[intersection[i].next[j]].distance) && (djik[intersection[i].next[j]].distance>0.0) && (djik[intersection[i].next[j]].status==0))
				{
					minr=djik[intersection[i].next[j]].distance;
					ref=intersection[i].next[j];


				}

			}
		}

		k=ref;

	}while(djik[goal].status==0);
	
	int v=0;
	int z=goal;

	do
	{
		z=djik[z].previous;
		v+=1;


	}while(z!=start);

	return v;

}
//path array
void pathset(Crossing cross[100], djikarr djik[100],int start,int goal,int num)
{
	int v,i;
	v=djikstra(cross,djik,start,goal,num);
	path[v+1]=-1;
	path[v] = goal;
	for(i=v-1;i>0;i--)
	{
		path[i]=djik[path[i+1]].previous;
	}
	path[0]=start;
}

/*function for plotting the intersections and roads*/
void opengl(Crossing cross[100],int start,int mark,int num, double speed, int k)
{
	djikarr djik[100];
	int crossing_number; /* The number of crossings */
	int vehicle_pathIterator; /* The position of the moving object on the path (it represents
				     that this is the n-th road) */
	double REAL_SIZE_X=20.0, REAL_SIZE_Y=20.0, ORIGIN_X=cross[start].pos.x, ORIGIN_Y=cross[start].pos.y;
	double x1, x0, y1, y0, x2, y2, panx, pany;
	

	int vehicle_stepOnEdge; /* The position of the moving object on the road (it represents that this is the n-th step taken on this road) */
	double vehicle_x =cross[start].pos.x, vehicle_y = cross[start].pos.y; /* The coordinate of the moving object */
	int vehicle_goal; /* The final destination point of the moving object */
	int vehicle_edgeFrom; /* The position of the moving object on the path 
				 (it represents that this is the n-th road) */
	int vehicle_edgeTo; /* The position of the moving object on the path 
			       (it represents that this is the n-th road) */

	int i,j;
	int width = 1000, height = 1000; /* Window size */
	double ang;
	double scale=0.1;

	/*Indicators of Mode*/
	int fps = 0;
	int Pan = 0, Rot = 0;
	
	

	/* Initialize the graphic environment and open a window */
	glfwInit();
	glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);


	setupfont();
	vehicle_pathIterator=0;
	vehicle_stepOnEdge=0;

	vehicle_edgeTo = vehicle_edgeFrom;
	vehicle_stepOnEdge = 0;

	pathset(cross,djik,start,mark,num);
	double distance; /* Direct distance */
	int steps;
	int signal;
	srand(time(NULL));

	int t=djik[mark].distance*100/speed;
	



	for(;;){




		/* The program is ended if the user pushes Esc or closes the window */
		if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			break;
		/* Keyboard controls for zoom and pan */

		if (Pan == 0 && Rot ==0)
		{
			if (fps%2 == 0)
			{	
				ORIGIN_X=vehicle_x;
				ORIGIN_Y=vehicle_y;
				ang = atan2(x1-x0,y1-y0)*180/3.142;
				panx = (x1-x0); 
				pany = (y1-y0)*0.1;
			}
			if (fps%2==1)
			{
			ang = 0;
				panx = 0; 
				pany = 0.2;
			}
			if (cross[mark].pos.x == vehicle_x && cross[mark].pos.y == vehicle_y)
			{
				panx = 0;
				pany = 0.2;
			}
			/* Set up a conversion matrix for projection */
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glRotatef(ang,0,0,1);
			glOrtho(ORIGIN_X+REAL_SIZE_X*-0.5,ORIGIN_X+REAL_SIZE_X*0.5,ORIGIN_Y+REAL_SIZE_Y*-0.5,ORIGIN_Y+REAL_SIZE_Y*0.5,-1.0,1.0);
			if(REAL_SIZE_X>8.0)
				REAL_SIZE_X*=0.99;
			if(REAL_SIZE_Y>8.0)
				REAL_SIZE_Y*=0.99;
			if(scale>0.018) scale*=0.97;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity(); /* Do not perform other coordinate conversion */

		}else if (Rot == 1 && Pan == 0)
		{
			ORIGIN_X=vehicle_x;
			ORIGIN_Y=vehicle_y;
			/* Set up a conversion matrix for projection */
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glRotatef(ang,0,0,1);
			glOrtho(ORIGIN_X+REAL_SIZE_X*-0.5,ORIGIN_X+REAL_SIZE_X*0.5,ORIGIN_Y+REAL_SIZE_Y*-0.5,ORIGIN_Y+REAL_SIZE_Y*0.5,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity(); /* Do not perform other coordinate conversion */	


		}else if (Pan == 1)
		{
			/* Based on (ORIGIN_X, ORIGIN_Y), project the space within the range of
			   REAL_SIZE_X * REAL_SIZE_Y to the viewport */
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glRotatef(ang,0.0,0.0,1.0);
			glOrtho(ORIGIN_X + REAL_SIZE_X * -0.5, ORIGIN_X + REAL_SIZE_X * 0.5,
					ORIGIN_Y + REAL_SIZE_Y * -0.5,ORIGIN_Y + REAL_SIZE_Y * 0.5,
					-1.0, 1.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity(); /* Do not perform other coordinate conversion */
		}


		/* Keyboard controls for zoom and pan */
		//Zoom
		if (glfwGetKey(GLFW_KEY_X) || !glfwGetWindowParam(GLFW_OPENED))
		{glScalef(0.9f, 0.9f, 0.9f && Pan == 0);REAL_SIZE_X=REAL_SIZE_X*1.1;REAL_SIZE_Y=REAL_SIZE_Y*1.1;}
		if (glfwGetKey(GLFW_KEY_Z) || !glfwGetWindowParam(GLFW_OPENED))
		{glScalef(1.1f, 1.1f, 1.1f && Pan == 0);REAL_SIZE_X=REAL_SIZE_X*0.9;REAL_SIZE_Y=REAL_SIZE_Y*0.9;}


		//Pan
		if (glfwGetKey(GLFW_KEY_D) || !glfwGetWindowParam(GLFW_OPENED))
		{ORIGIN_Y -= panx;ORIGIN_X +=pany;Pan = 1;}
		if (glfwGetKey(GLFW_KEY_A) || !glfwGetWindowParam(GLFW_OPENED))
		{ORIGIN_Y += panx;ORIGIN_X -=pany;Pan = 1;}
		if (glfwGetKey(GLFW_KEY_W) || !glfwGetWindowParam(GLFW_OPENED))
		{ORIGIN_X += panx;ORIGIN_Y +=pany;Pan = 1;}
		if (glfwGetKey(GLFW_KEY_S) || !glfwGetWindowParam(GLFW_OPENED))
		{ORIGIN_X -= panx;ORIGIN_Y -=pany;Pan = 1;}
		if (glfwGetKey(GLFW_KEY_Q) || !glfwGetWindowParam(GLFW_OPENED))
		{ang += 1;panx = panx*cos(0.017453292) + pany*sin(0.017453292);pany = pany*cos(0.017453292) - panx*sin(0.017453292);Rot = 1;}
		if (glfwGetKey(GLFW_KEY_E) || !glfwGetWindowParam(GLFW_OPENED))
		{ang -= 1;panx = panx*cos(-0.017453292) + pany*sin(-0.017453292);pany = pany*cos(-0.017453292) - panx*sin(-0.017453292);Rot = 1;}

		//Toggle/Return Mode
		if ((glfwGetKey(GLFW_KEY_R) || !glfwGetWindowParam(GLFW_OPENED)) && (Pan == 1 || Rot == 1)){Pan = 0; Rot = 0;}
		if (glfwGetKey(GLFW_KEY_T) || !glfwGetWindowParam(GLFW_OPENED)){fps += 1;}



		glfwGetWindowSize(&width,&height); /* Obtain the current window size */
		glViewport(0,0,width, height); /* Use the whole window as a viewport) */


		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT); /* Paint the back buffer in black */


		/* Draw different figures on the back buffer */
		glLineWidth(1.0);


		/*plots all the intersections and draws lines according to connection data*/
		for(i=0;i<num;i++){
			glColor3f(1.0,1.0,0.0);
			circle(cross[i].pos.x,cross[i].pos.y,0.05);

			for(j=0;j<cross[i].points;j++)   //loop for constructing the roads
			{
				glColor3d(0.0,1.0,1.0);
				line(cross[i].pos.x,cross[i].pos.y,cross[cross[i].next[j]].pos.x,cross[cross[i].next[j]].pos.y);
			}

		}

		//highlighting the path to destination
	
		i=1;
		while(path[i]!=-1)
		{	glLineWidth(2.0);
			glColor3d(1.0,1.0,1.0);
			line(cross[path[i-1]].pos.x,cross[path[i-1]].pos.y,cross[path[i]].pos.x,cross[path[i]].pos.y);
			glColor3d(0.0,0.5,0.6);
			outtextxy(cross[path[i-1]].pos.x,cross[path[i-1]].pos.y,cross[path[i-1]].name,ang,scale);

			i+=1;
		}
		glColor3f(0.0,1.0,0.0); //marks the start point
		marker(cross[start].pos.x,cross[start].pos.y,0.05);

		glColor3d(0.0,1.0,0.0);
		outtextxy(cross[start].pos.x,cross[start].pos.y,cross[start].name,ang,scale);

		glColor3f(1.0,0.0,0.0); //marks the final destination point
		marker(cross[mark].pos.x,cross[mark].pos.y,0.05);
		glColor3d(1.0,0.0,0.0);
		outtextxy(cross[mark].pos.x,cross[mark].pos.y,cross[mark].name,ang,scale);

		/* Move the moving object forward and calculate the coordinate */

		if (path[vehicle_pathIterator + 0] != -1 && path[vehicle_pathIterator + 1] != -1) {
			/* If neither the last crossing passed nor the next crossing is a dummy */
			/* Since the goal has not been reached, move forward the position of the moving object */
			/* Set (x0, y0), (x1, y1) for the current and next crossings */
			x0 = cross[path[vehicle_pathIterator + 0]].pos.x; /* The last crossing passed, x */
			y0 = cross[path[vehicle_pathIterator + 0]].pos.y; /* The last crossing passed, y */
			x1 = cross[path[vehicle_pathIterator + 1]].pos.x; /* The next crossing to pass, x */
			y1 = cross[path[vehicle_pathIterator + 1]].pos.y; /* The next crossing to pass, y */
			x2 = cross[path[vehicle_pathIterator + 2]].pos.x; /* The next crossing to pass, x */
			y2 = cross[path[vehicle_pathIterator + 2]].pos.y; /* The next crossing to pass, y */
			glColor3d(1.0,0.0,0.05);//highlighting current path
			line(x0,y0,x1,y1);
			glColor3d(1.0,.7,0.05);
			outtextxy(x1,y1,cross[path[vehicle_pathIterator + 1]].name,ang,scale);
			//outtextxy(x0,y0,cross[path[vehicle_pathIterator + 0]].name,ang,scale);

			distance = dist(x1 - x0, y1 - y0); /* Direct distance */
			double remdi=(djik[mark].distance-djik[path[vehicle_pathIterator + 0]].distance+dist(x1-vehicle_x,y1-vehicle_y))*100; //remaining distance
			double rtime=remdi/speed;  //Estimated time of Arrival
		        char p[100];	
			double dti=dist(x1-vehicle_x,y1-vehicle_y)*100; //distance to next intersection
			


			steps = (distance / 0.008);

			/* Move forward on the road and update the coordinate */
			vehicle_stepOnEdge++;

			vehicle_x = x0 + (x1 - x0) / steps * vehicle_stepOnEdge; 
			vehicle_y = y0 + (y1 - y0) / steps * vehicle_stepOnEdge;

			double m1=atan2(x1-x0,y1-y0)*180/3.14;
			double m2=atan2(x2-x1,y2-y1)*180/3.14;
			double turn=m2-m1;
			if(turn<0) turn=360+turn;
	

			      if(turn>10.0 && turn <180 && x1!=cross[mark].pos.x)
				{
				snprintf(p,100,"ETA %.2lf Turn Right at %.2lf",rtime,dti);
				glColor3d(1.0,1.0,0.0);
				outtextxy(vehicle_x,vehicle_y,p,ang,0.012);
				}
				if(turn>180.0 && turn <350 && x1!=cross[mark].pos.x)
				{
				snprintf(p,100,"ETA %.2lf Turn Left at %.2lf",rtime,dti);
				glColor3d(1.0,1.0,0.0);
				outtextxy(vehicle_x,vehicle_y,p,ang,0.012);
				}
				if(turn>350 || turn<10 ||x1==cross[mark].pos.x)
				{
				snprintf(p,100,"ETA %.2lf Continue Straight for %.2lf",rtime,dti);
				glColor3d(1.0,1.0,0.0);
				outtextxy(vehicle_x,vehicle_y,p,ang,0.012);
				}




			if (vehicle_stepOnEdge >= steps) { /* After reaching the crossing, enter the next road */
				signal=rand()%2;
				if(signal==0){
					glColor3f(1.0,0.0,0.0); //marks the final destination point
					marker(vehicle_x,vehicle_y,0.08);
				}
				else{
					glColor3f(0.0,1.0,0.0); //marks the final destination point
					marker(vehicle_x,vehicle_y,0.08);
				}


				vehicle_pathIterator+=1; /* Increment the index that represents the current crossing on the path */
				vehicle_stepOnEdge = 0; /* Reset the index that represents that it is the n-th step on the edge */
			}

		

		}

		/* Display the moving object at the current position */
		glColor3d(0.0, 5.0, 0.5);
		arrow(vehicle_x,vehicle_y,(x1-x0)*5/steps,(y1-y0)*5/steps);
	




		glfwSwapBuffers(); /* Swap the front buffer with the back buffer */ 
		if(signal==0){
			usleep(1000*1000);
			signal=1;
		}
		if(vehicle_x ==cross[mark].pos.x && vehicle_y==cross[mark].pos.y && k==1)
		{
			break;
		}


		usleep(t * 1000); /* Wait for about 100 millisecond */
	}



	glfwTerminate();

}

int searchmap(Crossing intersection[100], Position coordinates[100],int num)
{
	

	char looknm[256];
	int lookid;
	double lookpos[2];
	int f=0;

	int crit;
        int mark;

	printf("Select your search parameters->\n1) Name\n2) ID\n3) Coordinates\n\n~~~~Parameter~~~~=>");  //giving user options for selecting search parameters
        scanf(" %d",&crit);     //user inputs choice of parameters
	if(crit!=1 && crit!=2 && crit!=3)
	{
		printf("\nWrong input...program exiting!!!\n");
                exit(0);
	}

        /*name search*/
        if(crit==1)
        {
                printf("\nInput the name of the intersection(alphabets only)-> ");
                scanf(" %[^\n]",looknm);
                mark=namefind(intersection,looknm,num);

        }

        /*id search*/
        if(crit==2)
        {

                printf("\nInput the id of the intersection(integers only)-> ");
                scanf(" %d",&lookid);
                mark=idfind(intersection,lookid,num);


        }

        /*coordinates serach*/
        if(crit==3)
        {

                printf("\nInput the coordinates of the intersection with a space in between(real numbers only)-> ");
                scanf(" %lf %lf",&lookpos[0],&lookpos[1]);
                mark=posfind(intersection,lookpos,num);


        }
	return mark;

}



void main()
{
	int i,j,num,crit;
	/*calling the structures*/
	Crossing intersection[100];
	Position coordinates[100];
	djikarr djik[100];
	int start,mark,pit;

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
	printf("\n|****Initial Position****|\n\n");
	start=searchmap(intersection,coordinates,num);
	printf("\nDo you want to make a pit stop?(y/n)=>");
	char pitcho;
	scanf(" %c", &pitcho);
	if(pitcho!='y'&& pitcho!='n')
	{
		printf("Wrong input...program exiting!!!\n");
		exit(0);
	}

	if(pitcho=='y')
	{
		printf("\n|****Pit Stop****|\n\n");
		pit=searchmap(intersection,coordinates,num);
	}
	
	printf("\n|****Final Destination****|\n\n");
	mark=searchmap(intersection,coordinates,num);
	double speed;
	printf("Enter Cruise Control Mode Speed(speed limit is 150,50)--> ");
	scanf(" %lf",&speed);
	if(pitcho=='y')
	{
		opengl(intersection,start,pit,num,speed,1);	//calling function to plot the intersections and roads
		opengl(intersection,pit,mark,num,speed,2);
	}
	else opengl(intersection,start,mark,num,speed,2);


	printf("\nYou have arrived!\n");
	fclose(fp);
}

