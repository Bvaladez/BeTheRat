// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <ctime>
#include <cmath>
#include <cstring>
#include "stdlib.h" //stdlib has to be inlcuded before glut to ovveride glut exit
#include "glut.h"
#include "graphics.h"
#include "maze.h"
#include "mouse.h"

#include <iostream>




// Global Variables (Only what you need!)

bool glLeftButtonDown = false;
bool glRightButtonDown = false;
bool glMiddleButtonDown = false;

enum viewtype { top_view, perspective_view, rat_view };
viewtype current_view = top_view;
int getView = current_view;


double screen_x = 650;
double screen_y = 650;


Maze gMaze;
Mouse rat(gMaze.getStart(), 1.0, 1.2, .20, 0., 0., 0., 0., 90.);
	
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_TRIANGLE_STRIP);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawLine(double x1, double y1, double x2, double y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

void DrawQuad(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4)
{
	glBegin(GL_QUADS);
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y2, z2);
	
	glVertex3d(x2, y2, z2);
	glVertex3d(x3, y3, z3);
	
	glVertex3d(x3, y3, z3);
	glVertex3d(x4, y4, z4);
	
	glVertex3d(x4, y4, z4);
	glVertex3d(x1, y1, z1);

	glEnd();
}

void DrawQuadOutline(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4)
{
	glBegin(GL_LINES);
	glVertex3d(x1, y1, z1);
	glVertex3d(x2, y2, z2);
	
	glVertex3d(x2, y2, z2);
	glVertex3d(x3, y3, z3);
	
	glVertex3d(x3, y3, z3);
	glVertex3d(x4, y4, z4);
	
	glVertex3d(x4, y4, z4);
	glVertex3d(x1, y1, z1);

	glEnd();
}



void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}

double getTime() {
	static clock_t start_time = clock();
	clock_t current_time = clock();
	double t = double(current_time - start_time) / CLOCKS_PER_SEC;

	return t;
}


//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	if (current_view == perspective_view)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		//good for a maze that is 10 * 10
		gluLookAt(-3,-3, 17, (WIDTH/2) - 1, (HEIGHT/2) - 1, 0, 0, 0, 1);
	}
	else if (current_view == top_view)
	{
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
	}
	else // current_view == rat_view
	{
		gMaze.setView(2);
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		//double z_level = .25;
		double z_level = abs(sin(getTime()));
		double x = rat.getX();
		double y = rat.getY();
		double dx = rat.getDX();
		double dy = rat.getDY();
		double at_x = x + dx;
		double at_y = y + dy;
		double at_z = z_level;
		gluLookAt(x, y, z_level, at_x, at_y, at_z, 0, 0, 1);
		glutPostRedisplay();
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(100,100,255);
	
	
	gMaze.Draw();
	rat.drawRatInitialPos();

	glutSwapBuffers();
}


void SetTopView(int w, int h)
{
	// go into 2D mode
	gMaze.setView(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double world_margin_x = 0.5;
	double world_margin_y = 0.5;
	gluOrtho2D(-world_margin_x, WIDTH + world_margin_x,
		-world_margin_y, HEIGHT + world_margin_y);
	glMatrixMode(GL_MODELVIEW);
}

void SetPerspectiveView(int w, int h)
{
	gMaze.setView(1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 38.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ .1, /* Z far */ 40.0);
	glMatrixMode(GL_MODELVIEW);
}

void SetRatView(int w, int h)
{
	gMaze.setView(2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 38.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ .1, /* Z far */ 30.0);
	glMatrixMode(GL_MODELVIEW);
}


// This callback function gets called by the Glut
// system whenever a key is pressed.

void keyboardUP(unsigned char c, int x, int y) {
	switch (c) {
		
		case'w':
			glMiddleButtonDown = false;
			break;
		case'a':

			glLeftButtonDown = false;
			break;
	
		case'd':
			glRightButtonDown = false;
			break;

	}


}


void keyboard(unsigned char c, int x, int y)
{

	switch (c) 
	{


	case'w':
		glMiddleButtonDown = true;
		break;
	case'a':

		glLeftButtonDown =true;
		break;

	case'd':
		glRightButtonDown = true;
		break;



		case 27: // escape character means to quit the program
			exit(0);
			break;
		//exit superRatMode
		case 'n': 
			if (gMaze.getSuperRatMode()) {

				gMaze.setSuperRatMode(false);

				if (gMaze.isSafe(rat.getX(), rat.getY(), rat.getRadius())) {
					break;
				}
				else {

					gMaze.setSuperRatMode(true);
					break;
				}
			}
	
		//set SuperRatMode
		case 's':
			gMaze.setSuperRatMode(true);
			rat.setSpeed(.002);
			break;

		case 'r':
			current_view = rat_view;
			SetPerspectiveView(screen_x, screen_y);
			break;

		case 'p':
			current_view = perspective_view;
			SetPerspectiveView(screen_x, screen_y);
			break;

		case 't':
			current_view = top_view;
			SetTopView(screen_x, screen_y);
			break;

		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	if (current_view == top_view)
	{
		SetTopView(w, h);
	}
	else if (current_view == perspective_view)
	{
		SetPerspectiveView(w, h);
	}
	else // current_view == rat_view
	{
		SetPerspectiveView(w, h);
	}

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const double MARGIN = 0.5;
	gluOrtho2D(-MARGIN, WIDTH+MARGIN, -MARGIN, HEIGHT+MARGIN);
	glMatrixMode(GL_MODELVIEW);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		glLeftButtonDown = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		glLeftButtonDown = false;
	}

	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{
		glRightButtonDown = true;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP) 
	{
		glRightButtonDown = false;
	}

	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
		glMiddleButtonDown = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
		glMiddleButtonDown = false;
	}

	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff(){

}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("                                                                                           Rat Maze ");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUP);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
