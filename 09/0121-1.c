/*B7TB1714 Lisa Katoh*/
/*0121-1.c*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<unistd.h>
#include<GL/glfw.h>
#include<FTGL/ftgl.h>

#ifndef FONT_FILENAME
/*The file name of the fonts that are available on the training server*/
#define FONT_FILENAME "/home/anubrata/sem/09/roboto/Roboto-Condensed.ttf"
#endif/*A pointer that points to the read fonts*/

#define PathNumber 100/*The number of paths*/
#define Radius_Marker 0.1/*The radius of a marker*/

double ORIGIN_X;
double ORIGIN_Y;
double REAL_SIZE_X; 
double REAL_SIZE_Y;


/*Definition of data structure*/
typedef struct {double x,y;} position;/*position x,y*/

typedef struct {
	int id;/*Crossing number*/
	char name[50];/*Crossing name*/
	position pos;/*A structure that represents positions*/
	double wait;/*Average waiting time*/
	int points;/*The number of crossing roads*/
	int next[5];/*Crossing numbers of neighboring crossings*/
	double distance;
	int previous;
	int visited;
} Crossing;

Crossing crossing[100];/*The number of crossings = 100*/
int station[100];

FTGLfont *font; 
int path[PathNumber];

/*Create a function of Open Gl*/
void Open_window(void)
{
	/*Initialize the graphic environment and open a window*/
	glfwInit();
	glfwOpenWindow(1280, 800, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

	/*Set up a conversion matrix for projection
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  glOrtho(ORIGIN_X + REAL_SIZE_X * -0.5, ORIGIN_X + REAL_SIZE_X * 0.5, ORIGIN_Y + REAL_SIZE_Y *-0.5, ORIGIN_Y + REAL_SIZE_Y *0.5, -100,100);*Project the space with this range to a viewpoint*/

	//glScalef(1,1,1);
	//glRotatef(30.0,1.0,0,0);
	//glTranslatef(0,0,0);
	/*Set up a model view conversion matrix*/
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	return;
}

/*Initialize the contents of array of station*/
void initialize_station(void)
{
	int i; 
	int num=90;

	for(i=0;i<num;i++)
		station[i]=0;
	return;
}
/*Searching exact name match*/
int search_cross(void)
{
	int i,id;
	char buff[256];
	int f=0;/*Flag set when item is found*/
	int num=90;

	printf("Input the name of crossing of the position->");
	scanf("%s",buff);
	for(i=0;i<num; i++){/*Loop until the end of the data*/
		printf("we are searching %d %s\n", i, crossing[i].name);

		if(strcmp(crossing[i].name, buff)==0){/*if they are the same*/
			printf("Crossing name %s Coordinates %.2lf,%.2lf Distance from origin %.2lf\n",
					crossing[i].name, crossing[i].pos.x, crossing[i].pos.y, hypot(crossing[i].pos.x, crossing[i].pos.y));

			id=i;
			f=1;
			station[i]=1;/*To paint color*/
		}
	}
	return id;
}	

/*Searching partial name match*/
int search_cross_approx(void)
{
	int i;
	char buff[256];
	int f=0;/*Flag set when item is found*/
	int num=90;
	int id;
	int k;

	printf("Input the name of crossing of the position->");
	scanf("%s",buff);
	for(i=0;i<num; i++){/*Loop until the end of data*/
		if(strstr(crossing[i].name, buff)!=NULL){/*This is the only difference!*/
			printf("Crossing number %d: crossing %s with coordinates %.2lf,%.2lf and distance from origin %.2lf\n",
					crossing[i].id,crossing[i].name, crossing[i].pos.x, crossing[i].pos.y, hypot(crossing[i].pos.x, crossing[i].pos.y));
			f=1;
			station[i]=1;/*To paint color*/
		}
	}
	if (f==0) puts("Couldnt find crossing\n");

	printf("Which station are you looking for(Input ID)?\n");
	k=scanf(" %d",&id);
	if(k==0){//In the case that a person does not input the ID. 
		printf("Doesn't match.\n");
		exit(1);
	}

	return id;

}

/*Searching ID */
int search_cross_id(void)
{
	int i;
	int num=90;

	printf("Input the crossing ID of the crossing of the position(from zero) ->");
	scanf("%d",&i);
	if(i>num)
		printf("No crossing\n");
	else{
		printf("Crossing number %d is crossing %s with coordinates %.21f, %.21f. Distance from origin %.21f\n", (crossing[i].id), crossing[i].name, crossing[i].pos.x, crossing[i].pos.y, hypot(crossing[i].pos.x, crossing[i].pos.y));

		station[i]=1; /*To paint the color*/
	}
	return i;
}

/*The minimum distance station*/
int search_min_distance(void)
{
	int i,minindex=0;/*Provisional closest index*/
	double dist,mdist,X,Y;
	int num=90;

	printf("Input the coordinates of the point to search(Formatted %%lf %%lf)->");
	scanf("%lf %lf",&X,&Y);
	for(i=0;i<num; i++){/*Loop until end of data*/
		mdist=hypot(crossing[minindex].pos.x-X,crossing[minindex].pos.y-Y);/*Distance to current closest*/
		dist=hypot(crossing[i].pos.x-X,crossing[i].pos.y-Y);/*Distance to challenger*/
		if(dist<mdist)/*Challenger is closer*/
			minindex=i;/*Update closest index*/
	}
	printf("The crossing closest to point (%.2lf,%.2lf) is (distance %.2lf):\n", X,Y,hypot(crossing[minindex].pos.x-X,crossing[minindex].pos.y-Y));
	printf("Crossig name %s coordinates %.2lf, %.2lf Distance from origin %.2lf\n",crossing[minindex].name,crossing[minindex].pos.x,crossing[minindex].pos.y,hypot(crossing[minindex].pos.x,crossing[minindex].pos.y));

	station[minindex]=1;
	return minindex;
}

/*To choose which method to use in order to input datas*/
int search_crossing() {
	int k =0;
	int targetID;

	printf("Please choose the searching type to search for the crossing\n0: Exact Crossing Name, 1: Partial Crossing Name 2: Crossing Number, 3: Crossing Coordinates\n");
	scanf("%d",&k); 

	if(k==0) targetID=search_cross();
	else if(k==1) targetID=search_cross_approx();
	else if(k==2) targetID=search_cross_id();
	else if(k==3) targetID=search_min_distance();

	return targetID;
}

/*Search for the nearest crossing to the crossing goal among the nighboring crossings to the crossig id*/
//int search_nearest(int id, int last){
//   	int i; 
//   	int crossing_next;
//   	int num=90;
//   	double distance, min_distance=10000;/*Initialize with a large numerical value temporarily*/
//	for(i=0;i<crossing[id].points;i++) {
/*Calculate the distance between the i-th neighboring crossing of the crossing id and the crossing goal, then assign the value to distance. If the calculated distance is shorter than the previously set shortest distance (min_distance)...Complete the program*/
//		distance=hypot( (crossing[ (crossing[id].next[i]) ].pos.x - crossing[last].pos.x),
//						(crossing[ (crossing[id].next[i]) ].pos.y - crossing[last].pos.y) );
//  		if(distance <= min_distance) {
//			min_distance=distance;
//			crossing_next=crossing[id].next[i];
//		}
//	}
// return crossing_next;
//}

/*To define paths*/
//void path_set(int startID, int endID) {
//    int i;
//  path[0]=startID;
//  for(i=1 ;; i++) {
//		path[i]=search_nearest(path[i-1],endID);
//		if(path[i]==endID || (i-1) >= 30) {
//			path[i+1]=-1;
//			break;
//		}
//  }
//}

/*Draw a line segment */
void line(double x0, double y0, double x1, double y1)
{
	glBegin(GL_LINES);
	glVertex2d(x0, y0);
	glVertex2d(x1, y1);
	glEnd();
}


/*Draw a circle */
void circle(double x, double y, double r)
{
	int const N = 24; /*Draw a circle using line segments by dividing the circumference into 24 pieces */
	int i;

	glBegin(GL_LINE_LOOP);
	for (i=0;i<N;i++)
		glVertex2d(x+cos(2*M_PI*i/N)*r, y+sin(2*M_PI*i/N)*r);
	glEnd();
	return;
}

void car(double x, double y, double x1, double y1, double R, double G, double B)
{

	glLineWidth(2.5);
	glColor3d(R,G,B);
	circle(x-3*x1,y-3*y1,0.2);

	glColor3d(0.0,0.5,1.0);
	glBegin(GL_LINE_LOOP);
        glVertex2f(x,y);
        glVertex2f(x-4*x1+y1,y-4*y1-x1);
        glVertex2f(x-3*x1,y-3*y1);
        glVertex2f(x-4*x1-y1,y-4*y1+x1);

        glEnd();
}

int width, height;

/*To draw texts on the map*/ 
void setupfont() {
	font = ftglCreateExtrudeFont(FONT_FILENAME);
	if(font==NULL){
		perror(FONT_FILENAME);
		fprintf(stderr, "could not load font\n");
		exit(1);
	}
	ftglSetFontFaceSize(font, 12, 12);
	ftglSetFontDepth(font,0.01);
	ftglSetFontOutset(font, 0, 0.1);
	ftglSetFontCharMap(font,ft_encoding_unicode);
}

/*Draw strings*/
void outtextxy(double x, double y, char const *text, double angle, double scale){
//	double const scale = 0.005;
	glPushMatrix();
	glTranslated(x,y,0.0);
	glRotatef(-angle,0,0,1);
	glScaled(scale,scale,scale);
	ftglRenderFont(font,text,FTGL_RENDER_ALL);
	glPopMatrix();
}

/* Create a function to plot target crossings as red dots and others as white dots */
void plot_crossing_dots(void){
	int i;
	int num=90;

	glfwGetWindowSize(&width,&height); /* Obtain the current window size */
	glViewport(0,0,width, height); /* Use the window as a viewport */
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT); /*paint the black buffer in black */

	for(i=0;i<num;i++){
		glColor3d(1.0,1.0,1.0); /* Point the goal station as grean */
		circle(crossing[i].pos.x, crossing[i].pos.y, 0.1);
		/*Due to the big window(1280*800),amplify the coordinates by 30*/
		//glColor3d(1.0,1.0,1.0);
		//outtextxy(crossing[i].pos.x,crossing[i].pos.y,crossing[i].name);
	}
	return;
}

/*Plot roads on the map*/
void connect_crossing(void)
{
	int i,j;
	int width, height;
	int num=90;

	glfwGetWindowSize(&width,&height); /*Obtain the current window size */

	for(i=0;i<num;i++){
		for(j=0;j<crossing[i].points;j++){
			glColor3d(1.0,0.5,0.5); /* Paint the roads as red */
			line(crossing[i].pos.x, crossing[i].pos.y, crossing[crossing[i].next[j]].pos.x, crossing[crossing[i].next[j]].pos.y); /* Plot lines between all crossings with their neighbour crossings */
		}
	}
	return;
}

/*double distance_calcuration_of_neighbor(int i, int j)
  {
  for(i=0;i<90;i++){
  int crossingnum = crossing[i].points; 
  for(j=0;j<crossingnum;j++){
  return hypot(crossing[crossing[i].next[j]].pos.x-crossing[i].pos.x,crossing[crossing[i].next[j]].pos.y-crossing[i].pos.y);
  }
  }
  }*/

void dijkstra(int start, int destination){
	for(int i=0; i < 90; i++){
		crossing[i].distance = 1e1000;
		crossing[i].previous=-1;
		crossing[i].visited=0;
	}
	crossing[start].distance = 0;
	int unvisitedflag = 1;
	while(unvisitedflag=1){
		int min = -1;
		int minVal = 1e10000;
		unvisitedflag = 0;
		for(int i = 0; i<90; i++){
			if(crossing[i].visited == 0){
				unvisitedflag = 1;
			}
		}

		if(unvisitedflag == 0){
			return;
		}

		for(int i =0; i < 90; i++){
			if(crossing[i].distance < minVal && crossing[i].visited == 0){
				min = i;
				minVal = crossing[i].distance;
			}

		}
		//		printf("min should be %d\n",min);

		if(min == destination){
			int temp_path[90];
			int n;

			temp_path[0] = destination;
			for(n=1; temp_path[n-1]!=start; n++){
				temp_path[n] = 	crossing[temp_path[n-1]].previous;
			}
			for(int i=0; i<n; i++){
				path[i] = temp_path[n-i-1];
			}
			path[n] = -1;
			return;
		} 

		double x0 = crossing[min].pos.x;
		double y0 = crossing[min].pos.y;
		for(int j=0; j<crossing[min].points;j++){
			double x1 = crossing[crossing[min].next[j]].pos.x;
			double y1 = crossing[crossing[min].next[j]].pos.y;
			double alt = crossing[min].distance + hypot(x1-x0, y1-y0);
			//			printf("The neighbor nodes of the current node are %d\n",crossing[min].next[j]);

			if(alt < crossing[crossing[min].next[j]].distance){
				crossing[crossing[min].next[j]].distance = alt;
				crossing[crossing[min].next[j]].previous = min;
			}
		}
		crossing[min].visited= 1;

		//		for(int i=0;i<90;i++){
		//			printf("The current distance table shows %.2lf\n", crossing[i].distance);
		//			printf("The current visited arays show %d\n", visited[i]);
		//		}
	}
	return 0;
}

int main(void)
{
	/*Read the file*/
	int i=0,j,num;/*The number of crossings*/
	FILE *fp;
	int startID;/*The id number of start position*/
	int endID;/*The id number of final destination*/
	int steps;
	double x1, y1, x0, y0,x2, y2;
	srand(time(NULL));
	double scale=0.1;

	if((fp=fopen("map.dat","r"))==NULL) {       
		printf("File couldn't open\n");
		exit(1);
	}
	/*First, read the number of crossings*/
	fscanf(fp,"%d\n",&num);
	while(i<num) {
		fscanf(fp,"%d,%lf,%lf,%lf,%[^,],%d",&(crossing[i].id),&(crossing[i].pos.x),&(crossing[i].pos.y), &(crossing[i].wait), crossing[i].name, &(crossing[i].points));
		/*Write a program that uses the function fprintf to output the data of the structure to the file*/
		for(j=0;j<crossing[i].points;j++) {
			fscanf(fp,",%d",&(crossing[i].next[j]));
		}

		i++;
	}

	printf("%d\n",num);
	for(i=0;i<num;i++) {
		printf("%s ",crossing[i].name);
		printf("%lf %lf ",crossing[i].pos.x,crossing[i].pos.y);
		printf("%lf ",crossing[i].wait);
		printf("%d",crossing[i].points);
		for(j=0;j<crossing[i].points;j++) {
			printf("%d ",crossing[i].next[j]);
		}
		puts("");
	}
	fclose(fp);

	initialize_station(); /*initialize the crossings*/

	printf("\nThe initial station->");
	startID = search_crossing();/*To determine the id of the start position*/ 

	printf("\nThe final destination->");
	endID = search_crossing();/*To determine the id of the final destination*/
	Open_window();/*Open screen*/
	setupfont();

	double vehicle_x=crossing[startID].pos.x;
	double vehicle_y=crossing[startID].pos.y;
	double ang=0;

	/*Create and connect path*/
	//	path_set(startID,endID);
	//	for (i=0;path[i]!=-1;i++) {
	//		printf("path[%d] = %d\n", i, path[i]);
	//	}

	REAL_SIZE_X=20;//Variables to zoom in 
	REAL_SIZE_Y=20;

	dijkstra (startID, endID);
	int vehicle_pathIterator = 0;/*The position of the moving object on the path*/
	int vehicle_stepOnEdge = 0;/*The position of the moving object on the path*/
	int moving=0;//A variable shows whether objects stops or not
	for(i=0;i<90;i++){
		printf("startID=%d,path[%d]=%d\n",startID,i,path[i]);}

	for(;;){

		glLineWidth(1.0);
		/*The program is ended if the user pushes Esc or closes the window */
		if(glfwGetKey(GLFW_KEY_ESC)||!glfwGetWindowParam(GLFW_OPENED)) 
			break;
		/*In order for a screen to move as the vehicle starts to move*/		
		if(vehicle_x == 0 && vehicle_y == 0 &&  moving ==0)//First zoom in before a vehicle starts moving
		{
			ORIGIN_X = crossing[startID].pos.x, ORIGIN_Y = crossing[startID].pos.y;
			printf(" %lf : %lf\n", ORIGIN_X, ORIGIN_Y);
		}
		else
		{
			ORIGIN_X = vehicle_x, ORIGIN_Y = vehicle_y;//Then, a screen follows a vehicle
		}
		if(REAL_SIZE_X >8 && REAL_SIZE_Y>8)//Zoom in -0.25 every loop until the real size will become 8
		{	
			REAL_SIZE_X -= 0.25;
			REAL_SIZE_Y -= 0.25;
			scale*=0.965;
		}
		else//If it reaches 8, stop zooming in and start moving. 
		{
			REAL_SIZE_X=8;
			REAL_SIZE_Y=8;
			moving = 1;
		}
		
		/*Set up a conversion matrix for projection*/
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glRotatef(ang,0,0,1);
		// glTranslatef(ORIGIN_X, ORIGIN_Y,0.0f); 
		glOrtho(ORIGIN_X + REAL_SIZE_X * -0.5, ORIGIN_X + REAL_SIZE_X * 0.5, ORIGIN_Y + REAL_SIZE_Y *-0.5, ORIGIN_Y + REAL_SIZE_Y *0.5, -100,100);/*Project the space with this range to a viewpoint*/


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		plot_crossing_dots();
		connect_crossing();

		glColor3d(1.0,1.0,0.0);
		outtextxy(crossing[startID].pos.x,crossing[startID].pos.y,crossing[startID].name,ang, scale);

		glColor3d(0.0,1.0,1.0);
		outtextxy(crossing[endID].pos.x,crossing[endID].pos.y,crossing[endID].name,ang, scale);


i=1;
                while(path[i]!=-1)
                {       glLineWidth(2.8);
                        glColor3d(0.0,1.0,0.0);
                        line(crossing[path[i-1]].pos.x,crossing[path[i-1]].pos.y,crossing[path[i]].pos.x,crossing[path[i]].pos.y);
                        glColor3d(0.0,0.5,0.6);
                        outtextxy(crossing[path[i-1]].pos.x,crossing[path[i-1]].pos.y,crossing[path[i-1]].name,ang,scale);

                        i+=1;
                }
		//Animation
		if(moving==1)//A vehicle starts move, when a screen has zoomed in enough
		{
			if(path[vehicle_pathIterator] != -1 && path[vehicle_pathIterator + 1] != -1) {
				x0= crossing[path[vehicle_pathIterator+0]].pos.x;/*The last crossing passed, x*/
				y0= crossing[path[vehicle_pathIterator+0]].pos.y;/*The last crossing passed, y*/
				x1= crossing[path[vehicle_pathIterator + 1]].pos.x;/*The next crossing to pass, x*/
				y1= crossing[path[vehicle_pathIterator + 1]].pos.y;/*The next crossing to pass. y*/
				x2 = crossing[path[vehicle_pathIterator + 2]].pos.x; /* The next crossing to pass, x */
                        	y2 = crossing[path[vehicle_pathIterator + 2]].pos.y; /* The next crossing to pass, y */
                        	glLineWidth(2.8);
				glColor3d(1.0,0.0,0.05);//highlighting current path
                        	line(vehicle_x,vehicle_y,x1,y1);
                        	glColor3d(1.0,.7,0.05);//highlihting nsme of the next crossing
                        	outtextxy(x1,y1,crossing[path[vehicle_pathIterator + 1]].name,ang,scale);
				ang = (atan2(x1-x0,y1-y0))*180/3.14;
				
				double distance = hypot(x1-x0,y1-y0);/*The distance caluculated by Pythagorean theorem */
				steps= (int)(distance*100);/*100 steps in the distance between path[x] and path[x+1]*/

				vehicle_stepOnEdge++;
				vehicle_x = x0 + ( (x1-x0)*vehicle_stepOnEdge )/steps;
				vehicle_y = y0 + ((y1-y0)*vehicle_stepOnEdge)/steps; 
/*calculation of angle for turn indicator*/
				double m1=atan2(x1-x0,y1-y0)*180/3.14;
                        	double m2=atan2(x2-x1,y2-y1)*180/3.14;
                        	double turn=m2-m1;

				double dti=hypot(x1-vehicle_x,y1-vehicle_y)*100; //distance to next intersection

				char p[100];
				if(turn<0) turn=360+turn;


                              if(turn>15.0 && turn <180 && x1!=crossing[endID].pos.x)
                                {
                                snprintf(p,100,"Turn Right at %.2lf",dti);
                                glColor3d(1.0,1.0,0.0);
                                outtextxy(vehicle_x,vehicle_y,p,ang,0.012);
                                }
                                if(turn>180.0 && turn <345 && x1!=crossing[endID].pos.x)
                                {
                                snprintf(p,100,"Turn Left at %.2lf",dti);
                                glColor3d(1.0,1.0,0.0);
                                outtextxy(vehicle_x,vehicle_y,p,ang,0.012);
                                }
                                if(turn>350 || turn<10 ||x1==crossing[endID].pos.x)
                                {
                                snprintf(p,100,"Continue Straight for %.2lf",dti);
                                glColor3d(1.0,1.0,0.0);
                                outtextxy(vehicle_x,vehicle_y,p,ang,0.012);
                                }

				if(vehicle_stepOnEdge >= steps){/*After reaching the crossing, enter the mext road*/
					vehicle_stepOnEdge = 0;/*Reset the index that represents that it is the n-th step on the edge*/
					vehicle_pathIterator++;/*Increment the index that represents the current crossing on the path*/
				}
			}
		}
		/*Display the goal and the moving object*/
		double R=1.0,G=1.0,B=1.0;
		R=rand()%100;
		G=rand()%100;
		B=rand()%100;

		car(vehicle_x, vehicle_y, (x1-x0)*5/steps,(y1-y0)*5/steps,R/100,G/100,B/100);


		glfwSwapBuffers();
		usleep(20*1000);
		/*This has made images so far drawn visible*/
	}

	/*Close the windows before the program is ended (In case it is still open)*/
	glfwTerminate();/*(This does not have to be explicitly invoked)*/
	return 0;
}


