/* ex4-2.c */
/* compile: cc -o ex4-2 ex4-2.c -g -O2 -Wall -lglfw -lGLU -lGL -lX11 -lXrandr -lm */

#include <stdio.h>
#include <math.h> 
#include <unistd.h>
#include <GL/glfw.h>

int main(void) 
{
	glfwInit();
	glfwOpenWindow(640, 400, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.2, 2 * M_PI + 0.2, -1.2, 1.2, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (;;) {
		int width, height;
		double rad;

		glfwWaitEvents();
		if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			break; /* The program is ended if the uses pushes Esc or closes the window */

		glfwGetWindowSize(&width, &height); /* Obtain the current window size */
		glViewport(0, 0, width, height); /* Use the whole window as a viewport */

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
		glClear(GL_COLOR_BUFFER_BIT); /* Pant the back buffer in black */

		/* x-axis and y-axis */
		glColor3d(1.0, 1.0, 1.0); 
		glBegin(GL_LINES);
		glVertex2d(0.0, 0.0);
		glVertex2d(2 * M_PI, 0.0); 
		glVertex2d(0.0, -1.0);
		glVertex2d(0.0, 1.0); glEnd();

		/* Curve */
		glColor3d(0.5, 1.0, 0.5); 
		glBegin(GL_LINE_STRIP);
		for (rad=0; rad<=2*M_PI; rad += M_PI / 36) { 
			double s = sin(rad);
			printf("x:%f y:%f\n", rad, s); 
			glVertex2d(rad, s);
		}
		glEnd();

		glfwSwapBuffers(); /* Switch between the front and back buffers */
	}

	return 0; 
}
