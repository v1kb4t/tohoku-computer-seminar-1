/* ex5.c */
/* compile: cc -o ex5 ex5.c -g -O2 -Wall -lglfw -lGLU -lGL -lX11 -lXrandr -lm */

#include <stdio.h>
#include <math.h> 
#include <unistd.h>
#include <GL/glfw.h>

/* Function that behaves in the same way as fmin of C99 */
static double f_min(double v0, double v1)
{
	return (v0 < v1) ? v0 : v1;
}
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
	int width = 640, height = 400; /* Window size */
	double x = 200.0, y = 200.0; /* Central position of the ball */
	double vx = 50.0, vy = 50.0; /* Speed of the ball */
	double r = 10.0; /* Radius of the ball */

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
		
		/* Prevent the ball from going outside the window in case the window size is changed */
		x = f_min(x, width - r); 
		y = f_min(y, height - r);

		/* Move the ball */
		x += vx; 
		y += vy;

		/* Let the ball bounce if it hits one of the four sides of the window */
		if (x <= r || x >= width - r) 
			vx = -vx;
		if (y <= r || y >= height - r) 
			vy = -vy;

		glColor3d(1.0, 1.0, 1.0);
		circle(x, y, r); /* Draw a ball */

		glfwSwapBuffers(); /* Switch between the front and back buffers */
		usleep(40 * 1000); /* Wait about 40 milliseconds */
	}

	glfwTerminate(); /* Close the window */

	return 0; 
}
