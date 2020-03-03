#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "graphics.h"
#include "stdlib.h"
#include "glut.h"

class Mouse
{
public:
	Mouse(int start, double width, double height, double radius, double dx, double dy, double x, double y, double rotation);

	double getX();
	double getY();
	double getDX();
	double getDY();
	double getRadius();
	const double getRotation();
	const double getSpeed();
	
	void setSpeed(double speed);
	void addToRotation(double value);

	void drawRatInitialPos();
	void drawRatShape();
	//void drawRatShape(double width, double height);
	
	void scurryForward();

	void rotateRight();
	void rotateLeft();

private:

	double mWidth;
	double mHeight;
	int mStartPos;
	double mX;
	double mY;
	double mRotation;
	double mDx;
	double mDy;
	double mRadius;
	double mSpeed;
};


#endif
