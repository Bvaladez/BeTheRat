#include "stdlib.h"
#include "glut.h"
#include "mouse.h"
#include "maze.h"
#include "graphics.h"
#include "time.h"

#include <iostream>


Mouse::Mouse(int start, double width, double height, double radius, double dx, double dy, double x, double y, double rotation) {

	srand(time(0));
	
	mDx = dx;
	mDy = dy;
	mWidth = width;
	mHeight = height;
	mStartPos = start;
	mRadius = radius;
	mRotation = rotation;

	//the mouse should never start above the first row and will always start where the start of the maze is.
	mY = y;
	double centerMargin = .5;
	mX = x + mStartPos + centerMargin;
}

double Mouse::getX() {
	return mX ;
}

double Mouse::getY() {
	return mY;
}

double Mouse::getDX() {
	double degrees = this->getRotation();
	double radians = degrees * 3.145926 / 180.;
	double dx = std::cos(radians);
	return dx;
	//return mDx;
}

double Mouse::getDY() {
	double degrees = this->getRotation();
	double radians = degrees * 3.145926 / 180.;
	double dy = std::sin(radians);
	return dy;
	//return mDy;
}

double Mouse::getRadius() {
	return mRadius;
}

const double Mouse::getRotation() {
	return mRotation;
}

void Mouse::addToRotation(double value) {
	mRotation += value;
}

void Mouse::drawRatInitialPos() {

	extern bool glLeftButtonDown;
	extern bool glRightButtonDown;	
	extern bool glMiddleButtonDown;

	glPushMatrix();
	
	//glTranslated(-1 * (mStartPos + centerMargin) + -1.*this->getX(), -1.*this->getY(), 0);
	//CURRENT
	
	//DrawLine(mX - mDx, mY - mDy, mX + 1, mY +1);
	glTranslated(this->getX(), this->getY(), 0);
	//glTranslated(centerMargin, 0, 0);
	
	//only rotates around the z axis as this is a 2D representation.
	if (glLeftButtonDown) {
		rotateLeft();
	}

	if (glRightButtonDown) {
		rotateRight();
	}

	if (glMiddleButtonDown) {
		scurryForward();
	}
	
	//need to translate to the origin before rotation occurs
	
	//CURRENT
	glRotated(getRotation(), 0, 0, 1);
	
	//glTranslated(-1 * (mStartPos + centerMargin) + -1.*this->getX(), -1.*this->getY(), 0);
	
	glScaled(.3, .3, 1.0);

	glTranslated(0., 0., 0. );

	drawRatShape();
	
	glPopMatrix();

	//if (glLeftButtonDown) {
	//	rotateLeft();
	//}

}



// TODO: add parameters and data members of current point so it is not drawn statically in draw rat
void Mouse::drawRatShape() {


	//if (current_view == rat_view)
	//{
	//	return; // drawing yourself in rat view looks bad.
	//}
	
	
	DrawTriangle(.7, 0, -.5, .5, -.5, -.5);
}

void Mouse::scurryForward() {


	//double degrees = .01;
	//double degrees = 1;
	//subtract 90 from degrees

	double degrees = this->getRotation();

	double radians = degrees * 3.145926 / 180.;
	double dx = std::cos(radians);
	double dy = std::sin(radians);
	double speed = .001;

	//DrawLine(mX - (dx * speed), mY - (dy * speed), mX, mY);
	bool lastMoveLegalFlag = false;

	extern Maze gMaze;
	int i = this->getX();
	int j = this->getY();

	//win Condition	
	//if (j > HEIGHT - 1) {
	if (i == gMaze.getEndPoint() && j == HEIGHT - 1) {
		static int t = 0;
		static double rotation = 0;
		t += 1;
		rotation += .7;

		
		double r = 0;
		double g = 0;
		double b = 0;
		r = rand() % 100;
		g = rand() % 100;
		b = rand() % 100;
		r /= 100;
		g /= 100;
		b /= 100;
		

		glRotated(rotation, 0, 0, 1);
		if (t % 2 == 0 ) {
			glColor3d(r, g, b);
			//Crosshair
			DrawLine(0, 0, 0, 1);
			DrawLine(1, 0, 0, 0);
			DrawLine(0, 0, 0, -1);
			DrawLine(0, 0, -1, 0);
		}else{
		
			glColor3d(r, g, b);
			DrawLine(0, 0, 0, 2);
			DrawLine(2, 0, 0, 0);
			DrawLine(0, 0, 0, -2);
			DrawLine(0, 0, -2, 0);
		}
	}
	

	
	if (gMaze.isSafe(this->getX() + (dx * speed), this->getY() + (dy * speed), this->getRadius())) {
		
		this->mX += dx * speed;
		this-> mY += dy * speed;
		lastMoveLegalFlag = true;


	}

	//this will cause the rat to slide forward when he hits a wall.
	else if (gMaze.isSafe(this->getX() + (dx * speed), this->getY(), this->getRadius())) {
		if (gMaze.getEndPoint() != i && j < HEIGHT) {
			glColor3d(1, 0, 0);
		}
		this->mX += dx * speed;
	}
	else if (gMaze.isSafe(this->getX(), this->getY() + (dy * speed), this->getRadius())) {
		if (gMaze.getEndPoint() != i && j < HEIGHT) {
			glColor3d(1, 0, 0);
		}
		this->mY += dy * speed;
	}	//try to go just up down y += dy try just go left right

	glutPostRedisplay();
}


void Mouse::rotateRight() {

	//deltaRotation is negative so the rat will rotate right not left
	double deltaRotation = -.1;
	addToRotation(deltaRotation);
	glutPostRedisplay();
}


void Mouse::rotateLeft() {
	
	double deltaRotation = .1;
	addToRotation(deltaRotation);
	glutPostRedisplay();
}

