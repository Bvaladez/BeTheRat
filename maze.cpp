#include "stdlib.h" //stdlib has to be inlcuded before glut to ovveride its exit
#include "glut.h"
#include "graphics.h"
#include "maze.h"
#include "time.h"
#include "mouse.h"

#include<iostream>
#include <vector>

Cell::Cell()
{
	left = top = right = bottom = true;
	mVisited = false;
}

void Cell::visitCell() {
	this->mVisited = true;
}

bool Cell::isVisited() {
	if (this->mVisited == true) {
		return true;
	}
	return false;
}

void Cell::Draw3d(int x, int y)
{

		glColor3d(1, 0, 1);

		if (left) {
			int chaos = 425;
			int r = rcolorChaos(x, y, chaos);
			int g = gcolorChaos(x, y, chaos);
			int b = bcolorChaos(x, y, chaos);
			glColor3ub(r, g, b);

			DrawQuad(x, y, 0., x, y + 1, 0., x, y + 1, 1., x, y, 1.);
			

			glColor3ub(0, 0, 0);
			//DrawQuadOutline(x, y, 0., x, y + 1, 0., x, y + 1, 1., x, y, 1.);
		}
		if (top) {
			int chaos = 5;
			int r = rcolorChaos(x, y, chaos);
			int g = gcolorChaos(x, y, chaos);
			int b = bcolorChaos(x, y, chaos);
			glColor3ub(r, g, b);
		
			DrawQuad(x, y + 1, 0., x + 1, y + 1, 0., x + 1, y + 1, 1., x, y + 1, 1.);
		
			glColor3ub(0, 0, 0);
			//DrawQuadOutline(x, y + 1, 0., x + 1, y + 1, 0., x + 1, y + 1, 1., x, y + 1, 1.);
		}
		if (right && x == WIDTH-1) {
			
			int chaos = 0;
			int r = rcolorChaos(x, y, chaos);
			int g = gcolorChaos(x, y, chaos);
			int b = bcolorChaos(x, y, chaos);
			glColor3ub(r, g, b);
		
			DrawQuad(x + 1, y + 1, 0., x + 1, y, 0., x + 1, y, 1., x + 1, y + 1, 1.);
		
			glColor3ub(0, 0, 0);
			//DrawQuadOutline(x + 1, y + 1, 0., x + 1, y, 0., x + 1, y, 1., x + 1, y + 1, 1.);
		}
		if (bottom && y == 0) {
	
			int chaos = 39421;
			int r = rcolorChaos(x, y, chaos);
			int g = gcolorChaos(x, y, chaos);
			int b = bcolorChaos(x, y, chaos);
			glColor3ub(r, g, b);
	
			DrawQuad(x, y, 0., x + 1, y, 0., x + 1, y, 1., x, y, 1.);

			glColor3ub(0, 0, 0);
			//DrawQuadOutline(x, y, 0., x + 1, y, 0., x + 1, y, 1., x, y, 1.);
		}

		glColor3b(0, 0, 0);

}



void Cell::Draw2d(int x, int y)
{

	extern int getView;

	glColor3d(1, 0, 1);
	if (left)
		DrawLine(x, y, x, y + 1);
	if (top)
		DrawLine(x, y + 1, x + 1, y + 1);
	if (right)
		DrawLine(x + 1, y + 1, x + 1, y);
	if (bottom)
		DrawLine(x + 1, y, x, y);
	glColor3b(0, 0, 0);
	
}



Maze::Maze() {
	
	//each maze object will be seeded to a new time with this formatting	
	srand(time(0));

	mEndPoint = rand() % WIDTH;
	mStartPoint = rand() % WIDTH;

	RemoveWalls();

}

void Maze::setView(int view) {
	mView = view;
}

int Maze::getEndPoint() {
	return mEndPoint;
}

bool Maze::isSafe(double x, double y, double r){
	
	//throw away decimal to find current cell
	int i = (int)x;
	int j = (int)y;

	//std::cout << i << " i value " << std::endl;
	//std::cout << j << " j value " << std::endl;
	
	//take just the decimal or percent into current cell
	double xOffset = x - i;
	double yOffset = y - j;

	static int rightWall = 0;
	static int leftWall = 0;
	static int topWall = 0;
	static int bottomWall = 0;
	static int topRight = 0;
	static int topLeft = 0;
	static int bottomRight = 0;
	static int bottomLeft = 0;
	
	//check if a wall prohibits the rat from moving forward based on current cell	
	
	if ( cells[i][j].right && xOffset + r > 1.0) {
		//glColor3d(1, 0, 0);
		rightWall += 1;
			
		//std::cout << "Hit right wall " << rightWall << std::endl;
		return false;
	}
	else if (cells[i][j].left && xOffset - r < 0.) {
		//glColor3d(1, 0, 0);
		leftWall += 1;
		//std::cout << "Hit left wall " << leftWall << std::endl;
		return false;
	}
	else if (cells[i][j].top && yOffset + r > 1.0) {
		//glColor3d(1, 0, 0);
		topWall += 1;
		//std::cout << "Hit top wall " << topWall << std::endl;
		return false;
	}
	else if (cells[i][j].bottom && yOffset - r < 0.) {
		//glColor3d(1, 0, 0);
		bottomWall += 1;
		//std::cout << "Hit bottom wall " << bottomWall << std::endl;
		return false;
	}

	//check if the rat hits a corner, never allow movement into corners
	if (xOffset + r >= 1. && yOffset + r >= 1.) {
		//glColor3d(1, 0, 0);
		topRight += 1;
		//std::cout << "Hit Top right " << topRight <<  std::endl;
		return false;
	}
	else if (xOffset + r >= 1. && yOffset - r <= 0.) {
		//glColor3d(1, 0, 0);
		bottomRight += 1;
		//std::cout << "Hit bottom Right  " << bottomRight << std::endl;
		return false;
	}	
	else if (xOffset - r <= 0.  && yOffset - r <= 0.) {
		//glColor3d(1, 0, 0);
		bottomLeft += 1;
		//std::cout << "Hit Bottom Left " << bottomLeft << std::endl;
		return false;
	} 
	else if (xOffset - r <= 0. && yOffset + r >= 1. ) {
		//glColor3d(1, 0, 0);
		topLeft += 1;
		//std::cout << "Hit top left " << topLeft << std::endl;
		return false;
	}

	return true;
}


//recursivley removes walls carving out a maze then remove the top and bottom wall.
void Maze::RemoveWalls(){

	RemoveWallsR(0, 0);
	
	//remove a top and bottom wall for the start and the finsh.
	cells[this->mEndPoint][HEIGHT - 1].top = false;
	cells[this->mStartPoint][0].bottom = false;

}

void Maze::RemoveWallsR(int currentI ,int currentJ) {
	
	int i = currentI, j = currentJ;
	cells[i][j].visitCell();
	
	while (true) {
	
		//enum LEFT = 0, up = 1, Right = 2, down = 3. Allows me to call them by their names instead of memorizing number	
		enum MOVES {LEFT, UP, RIGHT, DOWN};
		std::vector<MOVES> moves;
		
		//check possible movement option add them to vector moves
		if (i - 1 >= 0 && !cells[i - 1][j].isVisited()) {
			moves.push_back(LEFT);
		}
		if (j + 1 < HEIGHT && !cells[i][j + 1].isVisited()) {
			moves.push_back(UP);
		}
		if (i + 1 < WIDTH && !cells[i + 1][j].isVisited()) {
			moves.push_back(RIGHT);
		}
		if (j - 1 >= 0 && !cells[i][j - 1].isVisited()) {
			moves.push_back(DOWN);
		}

		//if all possible moves are exhausted recurse back to last state
		if (moves.size() == 0) {
			return;
		}

			
		//randomly select a direction to travel, travel that way and carve out walls
		int r = rand() % moves.size();
		
		if (moves[r] == LEFT) {
			cells[i][j].left = false;
			cells[i - 1][j].right = false;
			RemoveWallsR(i - 1, j);
		}

		if (moves[r] == UP) {
			cells[i][j].top = false;
			cells[i][j + 1].bottom = false;
			RemoveWallsR(i, j + 1);
		}

		if (moves[r] == RIGHT) {
			cells[i][j].right = false;
			cells[i + 1][j].left = false;
			RemoveWallsR(i + 1, j);
		}

		if (moves[r] == DOWN) {
			cells[i][j].bottom = false;
			cells[i][j - 1].top = false;
			RemoveWallsR(i, j - 1);
		}
	}
}

int Maze::getStart(){
	return mStartPoint;
}


void Maze::Draw() {
	
	if(this->mView == 0){}
	for (int i = 0; i < WIDTH; i++){
		for (int j = 0; j < HEIGHT; j++){
			if(this->mView == 0 ){
				cells[i][j].Draw2d(i, j);
			}
			else {
				cells[i][j].Draw3d(i, j);
			}

		}
	}
}

int Maze::getView() {
	return mView;
}

int rcolorChaos(int x, int  y, int chaos) {
	//int a = x + rand();
	//int b = y + rand();

	int a = x + chaos;
	int b = y + chaos;
	
	if (x == 0) {
		a = 1;
	}
	if ( y == 0) {
		b = 1;
	}
	return (((a * 32241) + (b * 43882)) % 256);
}
int gcolorChaos(int x, int  y, int chaos) {
	//int a = x + rand();
	//int b = y + rand();
	int a = x + chaos;
	int b = y + chaos;
	
	if (x == 0) {
		a = 1;
	}
	if ( y == 0) {
		b = 1;
	}
	return (((a * 35194) + (b * 74287)) % 256) ;
}
int bcolorChaos(int x, int  y, int chaos) {
	int a = x + chaos;
	int b = y + chaos;
	
	if (x == 0) {
		a = 1;
	}
	if ( y == 0) {
		b = 1;
	}
	return (((a * 73451) + (b * 13212)) % 256);
}

