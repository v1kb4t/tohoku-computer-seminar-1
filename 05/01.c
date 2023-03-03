/* Draw lines in a window --- "test4.c" */
/* compile: cc test4.c -g -O2 -Wall -lgfw -lGLU -lGL -lX11 -lXrandr -lm -o test4 */

/* Use OpenGL to draw different figures.
 * - Use OpenGL to draw figures
 * - Use GLFW as the interface with the X system
 * Refer to google for reference materials
 *
 *
 * OpenGL has gone through major changes since OpenGL 2.0.
 * However, sample programs offered by this training only use old functionalities.
* So reference materials for OpenGL 1.0-1.5 are more useful. 
 */


#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <GL/glfw.h>

/* Draw a line segment */
void line(double x0, double y0, double x1, double y1)
{
  glBegin(GL_LINES);
  glVertex2d(x0,y0);
  glVertex2d(x1,y1);
  glEnd();
}

/* Draw a circle */
void circle(double x, double y, double r)
{
  int const N = 24; /* Draw a circle using line segments by dividing the circumference into 24 pieces */
  int i;

  glBegin(GL_LINE_LOOP);
  for (i=0;i<N;i++)
    glVertex2d(x+cos(2*M_PI*i/N)*r, y+sin(2*M_PI*i/N)*r);
  glEnd();
}

/* Draw a rectangle */
void rectangle(double x0, double y0, double x1, double y1)
{
  glBegin(GL_LINE_LOOP);
  glVertex2d(x0,y0);
  glVertex2d(x0,y1);
  glVertex2d(x1,y1);
  glVertex2d(x1,y0);
  glEnd();
}

void kite(double x0, double y0, double x1, double y1, double y2)
{
  glBegin(GL_LINE_LOOP);
  glVertex2d(x0,y0);
  glVertex2d(x1,y1);
  glVertex2d(x0,y2);
  glVertex2d(-x1,y1);
  glEnd();

}
int main(void)
{
  /* Initialize the graphic environment and open a window */
  glfwInit();
  glfwOpenWindow(640, 400, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

  /* Set up a conversion matrix for projection */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,640,0,400,-1.0,1.0); /* Project the space with this range to a viewport */

  /* Set up a model view conversion matrix */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  /* glScaled(0.4,0.4,0.4); /* Shrink it */
  /* glTranslated(-100.0,-100.0,0.0); /* Translate it */
  /* glRotated(15.0,0.0,0.0,1.0); /* Rotate it */

  for(;;){
    int width, height;
    
    /* Wait until an event occurs
     * Examples of a new event:
     * - A new window should be created (A new window is created, hidden parts have become visible, windows size is changed, etc)
     * - The user pushes a key or stops using a key
     */

    glfwWaitEvents();

    /* The program is ended if the user pushes Esc or closes the window */
    if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
      break;

    glfwGetWindowSize(&width,&height); /* Obtain the current window size */
    glViewport(0,0,width, height); /* Use the whole window as a viewport) */

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT); /* Paint the back buffer in black */

    /* Draw different figures on the back buffer */

    //glColor3d(0.5,1.0,0.5);
    //line(0,0,639,399);

   // glColor3d(0.5,1.0,1.0);
   // circle(320,200,100);

    //glColor3d(0.5,0.5,1.0);
    //glLineStipple(5,0xaaaa);
    //glEnable(GL_LINE_STIPPLE);
    //rectangle(160,50,479,349);
    //glDisable(GL_LINE_STIPPLE);

    glColor3d(0.5,1.0,0.5);
    kite(200,200,300,400,500);

    /* Switch between the front and back buffers */
    glfwSwapBuffers();
    /* This has made images drawn so far visible */
  }

  /* Close the windows before the program is ended (in case it is still open) */
  glfwTerminate(); /* (This does not have to be specifically invoked) */

  return 0;
}
