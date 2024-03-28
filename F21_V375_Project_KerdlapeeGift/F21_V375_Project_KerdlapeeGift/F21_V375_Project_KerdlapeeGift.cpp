// F21_V375_E5_GiftKerdlapee.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "Block.h"
#include "Utility.h"

#include <iostream>
#include <GL/freeglut.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream> // Use to convert a numeric type to string
#include <ctime>

using namespace std;

const float MINIMUM_MILLISECOND_UPDATE = (float).0001;

int gDrawMode = 1;

Utility MySettings;
Grid* MyGrid = new Grid;
vector <Grid*> MyWoodBlock;

void display();
void handleResize(int w, int h);
void myMenu(int value);
void handleKeypress(unsigned char key, int x, int y);
void mousebutton(int button, int state, int x, int y);
void mousebuttonPos( int x, int y);
void update();
void initialiseGrid(Grid* BlockPointer);
void drawGrid(Grid* BlockPointer);
void drawStatistics();
void displayString(string s, void* font);
double diffclock(clock_t clock1, clock_t clock2);
int timer();
void disableGrid(Grid* BlockPointer);
void enableGrid(Grid* BlockPointer);

clock_t gShowTime = 0;
double elapsedTime;
clock_t currentTime;
clock_t lastTime;
clock_t updateTime;
clock_t deltaTime;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	MySettings.initWindow();

	// Add a Menu
	glutCreateMenu(myMenu);
	glutAddMenuEntry("Pause", 1);
	glutAddMenuEntry("Reset", 2);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Add Mouse
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousebuttonPos);

	MySettings.createWoodBlocks();

	initialiseGrid(MyGrid);
	for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
	{
		initialiseGrid(MyWoodBlock[i]);
		//cout << MyWoodBlock[i]->getNoOfBlocks();
	}
	


	// draw scene
	glutDisplayFunc(display);
	glutReshapeFunc(handleResize);
	glutIdleFunc(update);
	glutKeyboardFunc(handleKeypress);
	glutMainLoop();

	return 0;
}

void initialiseGrid(Grid *BlockPointer)
{
	BlockPointer->initGrid();
}

void drawGrid(Grid* BlockPointer)
{
	BlockPointer->drawGrid();
}

// display()
// 
// This is were the action is taking place for drawing
//

void display()
{
	MySettings.doGraphicsInit();
	MySettings.doCamera();


	//PAUSED SCREEN
	if (gDrawMode == 0)
	{
		disableGrid(MyGrid);
		for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
			disableGrid(MyWoodBlock[i]);
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos2i(7, 5);

		string s = "  PAUSED";
		displayString(s, GLUT_BITMAP_9_BY_15);

		glRasterPos2i(5, 1);
		s = "    Press spacebar to continue";
		displayString(s, GLUT_BITMAP_9_BY_15);

		glPopMatrix();
	}

	//GAME SCREEN
	if (gDrawMode == 1)
	{
		enableGrid(MyGrid);
		for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
			enableGrid(MyWoodBlock[i]);
		drawGrid(MyGrid);
		for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
			drawGrid(MyWoodBlock[i]);
		drawStatistics();
	}
	//RESULT SCREEN
	else if (gDrawMode == 2)
	{
		disableGrid(MyGrid);
		for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
			disableGrid(MyWoodBlock[i]);

		//Print final score
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos2i(5, 5);

		string s = "Your final score: ";
		displayString(s, GLUT_BITMAP_9_BY_15);

		stringstream out;
		out << MyGrid->score;
		s = out.str();
		displayString(s, GLUT_BITMAP_9_BY_15);

		glRasterPos2i(5, 1);
		s = "     Press spacebar to restart";
		displayString(s, GLUT_BITMAP_9_BY_15);

		glPopMatrix();

	}


	glutSwapBuffers();
}

void disableGrid(Grid* BlockPointer)
{
	BlockPointer->isDisabled = true;
}

void enableGrid(Grid* BlockPointer)
{
	BlockPointer->isDisabled = false;
}

//function that prints score, error, and stopwatch
void drawStatistics()
{
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i(14, 8);

	string s = "Score = ";
	displayString(s, GLUT_BITMAP_9_BY_15);

	stringstream out;
	out << MyGrid->score;
	s = out.str();
	displayString(s, GLUT_BITMAP_9_BY_15);


	glColor3f(0.6f, 0.1f, 0.0f);
	glRasterPos2i(15, 9);
	stringstream out3;
	out3 << timer();
	s = out3.str();
	displayString(s, GLUT_BITMAP_HELVETICA_18);

	glPopMatrix();
}


// function that takes in a string and a font type to draw on screen texts
void displayString(string s, void* font)
{
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
}

//function that counts down in second using gShowtime
int timer()
{
	int stopWatch = 15 - gShowTime / 1000;
	return stopWatch;
}

// Reshape Callback Function
void handleResize(int w, int h)
{
	MySettings.windowWidth = w;
	MySettings.windowHeight = h;
	MySettings.doCameraOrthographic(w, h);
}

void update()
{
	//upon initialisation, store these variables to keep track of time
	if (gShowTime == 0)
	{
		deltaTime = 0; //delta time is the time difference (used when game is paused)
		updateTime = clock(); //update time is the time when the graphic was displayed
		lastTime = clock(); //last time is the first frame of time we start the table
		gShowTime = clock(); //gShowTime is the time that has actually elapsed in game
	}
	else
	{
		currentTime = clock();
		elapsedTime = double(diffclock(currentTime, updateTime)) / 1000.0; // time difference between each update

		if (elapsedTime > MINIMUM_MILLISECOND_UPDATE)
		{
			if (gDrawMode == 1)
			{
				gShowTime = currentTime - lastTime + deltaTime;

			}

			//when game is paused, record the delta time
			else if (gDrawMode == 0)
			{
				deltaTime = gShowTime;
				lastTime = currentTime;
			}

			//when timer stops, show result
			if (gShowTime >= 15 * 1000.0)
			{
				gDrawMode = 2;
			}
			glutPostRedisplay();
			updateTime = currentTime; //update time after scene is displayed
		}

	}

}

double diffclock(clock_t clock1, clock_t clock2)
{
	double diffticks = clock1 - clock2;
	double diffms = (diffticks * 1000) / CLOCKS_PER_SEC;
	return diffms;
}

//// Note that mousebutton will detect both up and down of the mouse button click
void mousebutton(int button, int state, int x, int y)
{
	MySettings.mousebutton(button, state, x, y);

}

void mousebuttonPos(int x, int y)
{
	MySettings.mousebuttonPos(x, y);

}

// myMenu
//
// Handles cases of user input from keyboard for menu choices
//
void myMenu(int value)
{
	switch (value)
	{
	case 1:
		gDrawMode = 0;
		glutPostRedisplay(); // Marks the current window for needing re-display
		break;
	case 2:
		MyGrid->reset();
		gDrawMode = 1;
		initialiseGrid(MyGrid);
		MyWoodBlock.clear();
		MySettings.createWoodBlocks();
		glutPostRedisplay(); // Marks the current window for needing re-display
		break;
	case 3:
		exit(0);
		break;
	}
}

// handleKeypress
// function to handle any key pressed by the user, for now just quits the program when user types ESC
void handleKeypress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape Key
		exit(0);
		break;
	case 32://space key

		//press spacebar to continue playing
		if (gDrawMode == 0)
		{
			gDrawMode = 1;
		}

		//press spacebar to restart
		else if (gDrawMode == 2)
		{

			MyGrid->reset();
			gDrawMode = 1;
			initialiseGrid(MyGrid);
			glutPostRedisplay(); // Marks the current window for needing re-display

		}
		break;

			
	}



}