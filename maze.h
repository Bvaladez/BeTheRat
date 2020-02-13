#pragma once
#include "graphics.h"


const int WIDTH = 10;
const int HEIGHT = 10;

struct Cell
{
	Cell();
	void Draw2d(int x, int y);
	void Draw3d(int x, int y);
	bool left, top, right, bottom;

	void visitCell();
	bool isVisited();

private:
	bool mVisited;
};

class Maze
{
public:
	Maze();
	
	void setView(int view);

	int getEndPoint();
	void RemoveWalls();
	void RemoveWallsR(int currentI, int currentJ);
	void Draw();
	int getStart();
	bool isSafe(double x, double y, double r);



	

private:
	Cell cells[WIDTH][HEIGHT];
	int mStartPoint;
	int mEndPoint;
	int mView;
	
};