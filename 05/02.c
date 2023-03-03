//system of star, planet and satellite

#include <stdio.h>
#include <math.h> 
#include <unistd.h>
#include <GL/glfw.h>


/* Draw a circle */
void circle(double x, double y, double r)
{
	int const N = 24; /* Draw a circle using line segments by dividing the circumference into 24 pieces */
	int i;

	glBegin(GL_LINE_LOOP);
	for (i = 0; i < N; i++)
		glVertex2d(x + cos(2 * M_PI * i / N) * r, y + sin(2 * M_PI * i / N) * r);
	glEnd(); 
}


int main(void)
{
	int width = 1600, height = 900; /* Window size */
	double x = 850.0, y = 500.0;  //initial position of planet 
	double x1=880, y1 = 500;	//initial position of sattellite
	double vx=0.0, vy=4.0;	//initial velocity of planet
        double vy1=0.5, vx1=0.0;	//initial velocity of satellite

	double r = 10.0; /* circle */
	double G = 6.67*pow(10,-11);	//value of universal gravitation constant in S.I.
	double M = 1.8*pow(10,14);	//mass of star
        double m1=1.5*pow(10,11);	//mass of planet
	double F,ang,c,d,ax,ay,c1,d1,ang1,ax1, ay1,f; 



	/* Initialize the graphic environment and open a window */
	glfwInit();
	glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); /* Do not convert coordinates except for projection purposes */

	for (;;) {
		/* The program is ended if the user pushes Esc or closes the window */
		if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)) 
			break;

		glfwGetWindowSize(&width, &height); /* Obtain the current window size */
		glViewport(0, 0, width, height); /* Use the whole window as a viewport */

		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glOrtho(0, width, 0, height, -1.0, 1.0); /* Project the space with this range to a viewport */

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
		glClear(GL_COLOR_BUFFER_BIT); /* Paint the back buffer in black */

		c=x-500;	//horizontal displacement of planet and sun
		d=y-500;	//vertical displacement of planet and sun
		c1=x1-x;	//horizontal displacement of planet and satellite
		d1=y1-y;	//vertical displacement of planet and satellite
		
		ang=atan(d/c);	//angle subtended by planet at the star, first quadrant value
		if(d>0&&c<0) ang = (3.14)+ang;	//2nd quadrant
		if(d<0&&c<0) ang= 3.14 + ang;	//3rd quadrant
		if(d<0&&c>0) ang=2*3.14 + ang;	//4th quadrant
		
		ang1=atan(d1/c1);	//angle subtended by satellite at the planet, first quadrant value
		if(d1>0&&c1<0) ang1 = (3.14)+ang1;	//2nd quadrant
		if(d1<0&&c1<0) ang1= 3.14 + ang1;	//3rd quadrant
		if(d1<0&&c1>0) ang1=2*3.14 + ang1;	//4th quadrant


		/* Accelerate the planet and the satellite */
		F = -G*M/(pow(c,2)+pow(d,2));	//acceleration of the planet due to star's gravity
		ax=cos(ang)*F;	//horizontal component of star's gravitational acceleration
		ay=sin(ang)*F;	//vertical component of star's gravitational acceleration

		f=-G*m1/(pow(c1,2)+pow(d1,2));	//acceleration of the satellite due to planet's gravity
		ax1=cos(ang1)*f;	//horizontal component of planet's gravitational acceleration
		ay1=sin(ang1)*f;	//horizontal component of planet's gravitational acceleration
	
		vx+=ax;	//update the planet's velocity in horizontal direction
		vy+=ay;	//update the planet's velocity in vertical direction

		vx1+=ax1;	//update the satellite's velocity in horizontal direction
		vy1+=ay1;	//update the satellite's velocity in vertical direction


		/* Move the planet and satellite */
		x += vx;	//update horizontal position of planet
		y += vy;	//update vertical position of planet

		x1 += vx1+vx;	//update horizontal position of satellite
		y1 += vy1+vy;	//update vertical position of satellite


		//planet
		glColor3d(0.0, 1.0, 0.0);
		circle(x, y, r); 

		//star
		glColor3d(1.0, 1.0, 0.0);
		circle(500,500, 3*r);
		
		//satellite
		glColor3d(1.0, 1.0, 1.0);
		circle(x1, y1, r/4);

		glfwSwapBuffers(); /* Switch between the front and back buffers */
		usleep(40*1000); /* Wait about 40 milliseconds */
	}

	glfwTerminate(); /* Close the window */

	return 0; 
}
