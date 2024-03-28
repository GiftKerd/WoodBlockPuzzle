
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <math.h>
#include <ctime>

// Include other classes required
#include "stdafx.h"
#include "Utility.h"
#include "Block.h"

//Grab access to the global list of pointers to objects
extern Grid* MyGrid;
extern vector <Grid*> MyWoodBlock;
extern Utility MySettings;

using namespace std;

//CONSTRUCTOR
Utility::Utility()
{
	windowWidth = 850;
	windowHeight = 512;
	remapx = 0;
	remapy = 0;
	nMousex = 0;
	nMousey = 0;
	aspect = 1;
	isVertical = true;
}

//
//doGraphicsInit
//
//initialise graphics
void Utility::doGraphicsInit()
{
	glMatrixMode(GL_MODELVIEW); // Switch to the drawing perspective
	doCamera();

	// Clear information from last draw
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	srand(time(0));
}


//
//doCamera
//
// Reset the drawing perspective
void Utility::doCamera()
{
	glLoadIdentity(); // Reset the drawing perspective
}

// Sets up a Window 
//
// The default is a window with center  0, 0, 0 and goes out to -1 and 1, 
// this is the default glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );
//
// By default it will open in the top left corner, small size 300 by 300
//
// To change these defaults you can use the functions, for example
// glutInitWindowSize( 500, 500 );
// glutInitWindowPosition( 100, 100 );
//
void Utility::initWindow()
{
	// This creates the window with a given title
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Hello");
	//set the ortho height and width
	glOrtho(0, ORTHOHEIGHT, 0, ORTHOHEIGHT, 0, 1.0);

	// select clearing background color
	glClearColor(0.0, 1.0, 0.0, 0.0);

}


//this function is for glutReshapeFunc(handleResize)
void Utility::doCameraOrthographic(int w, int h)
{
	if (h == 0)
	{
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	 aspect = (float)w / (float)h;
	float left, bottom;
	bottom = left = 0;

	if (isVertical = w <= h)
	{
		glOrtho((double)left, ORTHOHEIGHT, bottom / aspect, ORTHOHEIGHT / aspect, 0.0, 1.0);
		//isHorizontal = false;
	}
		
	else
	{
		glOrtho((double)left * aspect, (double)ORTHOHEIGHT * aspect, bottom, ORTHOHEIGHT, 0.0, 1.0);
		//isHorizontal = true;
	}
		

}

// Note that mousebutton will detect both up and down of the mouse button click
void Utility::mousebutton(int button, int state, int x, int y)
{
	if (!isVertical)
	{
		remapx = (float)windowWidth / (ORTHOHEIGHT * aspect);
		remapy = (float)windowHeight / ORTHOHEIGHT;
	}
	else
	{
		remapx = (float)windowWidth / (ORTHOHEIGHT / aspect);
		remapy = (float)windowHeight / ORTHOHEIGHT;
	}


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		//remapping the location of the mouse in pixel to ortho
		//cout << "\nMouse values are (x,y): " << x << " " << y;
		nMousex = x / remapx;
		nMousey = ORTHOHEIGHT - y / remapy;
		//cout << "\nNew ouse values are (x,y): " << nMousex << " " << nMousey;

		//check if the mouse is on the wood blocks
		//MyWoodBlock->checkClick();

	//	//if so draw the next table
	//	if (MyTable.correct)
	//	{
	//		MyTable.initTable();
		for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
		{
			MyWoodBlock[i]->checkClick();

			for (unsigned int j = 0; j < MyWoodBlock.size(); j++)
			{
				if (i == j)
					continue;
				if (MyWoodBlock[i]->isClicked)
				{
					MyWoodBlock[j]->isClicked = false;
					glutSetCursor(GLUT_CURSOR_NONE);
				}
			}
		}

	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{


		//cout << "\nresize back\n";
		for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
		{
			if (MyWoodBlock[i]->isClicked)
			{
				MyGrid->checkClick();
			}
			MyWoodBlock[i]->isClicked = false;
			MyWoodBlock[i]->initGrid();
		}
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		
	}
}

// Note that mousebutton will detect both up and down of the mouse button click
void Utility::mousebuttonPos(int x, int y)
{
	if (!isVertical)
	{
		remapx = (float)windowWidth / (ORTHOHEIGHT * aspect);
		remapy = (float)windowHeight / ORTHOHEIGHT;
	}
	else
	{
		remapx = (float)windowWidth / (ORTHOHEIGHT / aspect);
		remapy = (float)windowHeight / ORTHOHEIGHT;
	}


		//remapping the location of the mouse in pixel to ortho
		//cout << "\nMouse values are (x,y): " << x << " " << y;
		nMousex = x / remapx;
		nMousey = ORTHOHEIGHT - y / remapy;
		//cout << "\nNew ouse values are (x,y): " << nMousex << " " << nMousey;

		//check if the mouse is on the wood blocks
		//cout << MyWoodBlock->gridBlocks[0][0].position[0];
		for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
		{
			if (MyWoodBlock[i]->isClicked)
			{
				MyWoodBlock[i]->updateGrid();
				MyGrid->computeIntersection();
			}
			else
			{
				MyWoodBlock[i]->initGrid();
			}

		}

		

		//	//if so draw the next table
		//	if (MyTable.correct)
		//	{
		//		MyTable.initTable();
}

void Utility::createWoodBlocks()
{
	MyWoodBlock.push_back(new WoodBlock(1+rand () % 3,0,0));
	MyWoodBlock.push_back(new WoodBlock(1+rand() % 2,3.25,0));
	MyWoodBlock.push_back(new WoodBlock(1+rand() % 2,6.5,0));
}


