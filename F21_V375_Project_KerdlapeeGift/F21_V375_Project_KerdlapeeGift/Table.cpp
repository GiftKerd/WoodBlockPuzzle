
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <math.h>
#include <ctime>
#include <cstdlib>

// Include other classes required
#include "table.h"
#include "stdafx.h"
#include "Utility.h"

// Grab access to the global list of pointers to objects
//extern Utility MySettings;
//extern clock_t gShowTime;

using namespace std;

//CONSTRUCTOR
Table::Table()
{
	gap = 0.05f;
	margin = 0.6f;
	tableSize = 1;
	variantPos[0] = 0;
	variantPos[1] = 1;
	variant = 0.1;

	boxSize = 0;
	xval = 0;
	yval = 0;

	correct = false;

	colourSwitch = 0;
	colour[0] = 1.0;
	colour[1] = 1.0;
	colour[2] = 0.0;
	srand(time(0));

	score = 0;
	error = 0;

	disabled = false;
}


//function that draws the table
void Table::drawTable()
{
	double colour1, colour2, colour3;
	float yvalCheck, xvalCheck;

	//draw table
	for (int j = 0; j < tableSize; j++) //columns
	{
		//adjust the size of each box according to the size of the table
		boxSize = (ORTHOHEIGHT - (tableSize + 1) * gap - margin * 3) / tableSize;

		//y translate
		yvalCheck = j * boxSize + gap * (j + 1) + margin;

		for (int i = 0; i < tableSize; i++) //rows
		{
			//x translate
			xvalCheck = i * boxSize + gap * (i + 1) + margin * 1.5;

			//draw boxes
			glPushMatrix();
			glTranslatef(xvalCheck, yvalCheck, 0.0);

			//change the colour of the variant box
			if (i == variantPos[0] && j == variantPos[1])
			{
				colour1 = colour[0] - variant;
				colour2 = colour[1] - variant;
				colour3 = colour[2] - variant;
				yval = j * boxSize + gap * (j + 1) + margin;
				xval = i * boxSize + gap * (i + 1) + margin * 1.5;
			}

			//apply the normal colour to the regular boxes
			else
			{
				colour1 = colour[0];
				colour2 = colour[1];
				colour3 = colour[2];
			}

			//draw square
			solidSquare(boxSize, colour1, colour2, colour3);
			glPopMatrix();
		}
	}
	//glTranslatef(xval, yval, 0.0);
	//glutSolidSphere(.3, 20, 20);
}

//
//solidSquare
//
//function that uses size, and colours to draw square
//void solidSquare(float size, float colour1, float colour2, float colour3)
//{
//	// set the color for the rectangle
//	glColor3f(colour1, colour2, colour3);
//
//	// draw a rectangle with corners at ( .25, .25, 0 ) and ( .75, .75, 0 )
//	glBegin(GL_POLYGON);
//	glVertex3f(0.0, 0.0, 0.0);
//	glVertex3f(size, 0.0, 0.0);
//	glVertex3f(size, size, 0.0);
//	glVertex3f(0.0, size, 0.0);
//	glEnd();
//}
//
//
////
////checkClick
////
////function that determines if the mouse is on the correct box
//void Table::checkClick()
//{
//	//if the game is disabled, do not run this function
//	if (disabled)
//	{
//		return;
//	}
//
//	//checks if mouse is within the area of the variant box
//	if (MySettings.nMousex >= margin * 1.5 + gap && MySettings.nMousex <= ORTHOHEIGHT - margin * 1.5 - gap &&
//		MySettings.nMousey >= gap + margin && MySettings.nMousey <= ORTHOHEIGHT - margin * 2 - gap)
//	{
//		if (MySettings.nMousex >= xval && MySettings.nMousex <= xval + boxSize &&
//			MySettings.nMousey >= yval && MySettings.nMousey <= yval + boxSize)
//		{
//			//cout << "\n" << xvalCheck + boxSize << ", " << xvalCheck << "\n" << yvalCheck + boxSize << ", " << yvalCheck;
//			cout << "\nscore: " << score;
//			correct = true;
//			score++;
//		}
//		else
//		{
//			cout << "\nerror: " << error;
//			correct = false;
//			error++;
//		}
//	}
//
//}