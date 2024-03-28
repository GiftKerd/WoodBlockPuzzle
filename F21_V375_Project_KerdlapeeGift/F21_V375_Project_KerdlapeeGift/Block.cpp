
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
extern Grid* MyWoodBlockArray;
extern clock_t gShowTime;

bool checkDuplicate(vector <int> v, int num);
//CONSTRUCTORS FOR Block, Grid, WoodBlock

Block::Block()
{
	boxSize = 0;
	position[0] = 0;
	position[1] = 0;
	colour[0] = 1;
	colour[1] = 0;
	colour[2] = 0;
	isExisting = false;
	isOccupied = false;
	isHovered = false;
}

Block::Block(float posX, float posY, float size)
{
	boxSize = size;
	position[0] = posX;
	position[1] = posY;
	colour[0] = 1;
	colour[1] = 0;
	colour[2] = 0;
	isExisting = false;
	isOccupied = false;
	isHovered = false;
}

Grid::Grid()
{
	colourCounter = 0;
	gap = .1f;
	margin = .5f;
	tableSize = CONST_TABLESIZE;
	boxSize = (ORTHOHEIGHT - gap * (CONST_TABLESIZE + 1) - margin * 2) / CONST_TABLESIZE;
	isClicked = false;
	clickedWoodB = 100;
	offsetY = 0;
	isPlaced = false;
	isDisabled = false;
	score = 0;
	numOfWoodBlock = 0;
	for (int j = 0; j < tableSize; j++) //columns
	{

		for (int i = 0; i < tableSize; i++) //rows
		{

			gridBlocks[j][i] = Block();
			gridBlocks[j][i].isExisting = true;
		}
	}

}

Grid::Grid(float size)
{
	colourCounter = 0;
	gap = .1f;
	margin = .5f;
	tableSize = CONST_TABLESIZE;
	boxSize = size;
	isClicked = false;
	clickedWoodB = 100;
	offsetY = 0;
	isPlaced = false;
	isDisabled = false;
	score = 0;
	numOfWoodBlock = 0;
	for (int j = 0; j < tableSize; j++)
	{

		for (int i = 0; i < tableSize; i++)
		{

			gridBlocks[j][i] = Block();
		}
	}

}


WoodBlock::WoodBlock()
{
	tableSize = 2;

	colour[0] = 0;
	colour[1] = 1;
	colour[2] = 0;

	for (int j = 0; j < tableSize; j++)
	{

		for (int i = 0; i < tableSize; i++)
		{
			gridBlocks[j][i] = Block();
		}
	}

	gap = .1f;
	margin = .5f;
	boxSize = .7f;
	randomisedShape = 0;

}

WoodBlock::WoodBlock(int tablesize, float offset, int rand)
{
	tableSize = tablesize;

	colour[0] = 0;
	colour[1] = 1;
	colour[2] = 0;

	for (int j = 0; j < tableSize; j++)
	{

		for (int i = 0; i < tableSize; i++)
		{
			gridBlocks[j][i] = Block();
		}
	}

	gap = .1f;
	margin = .5f;
	boxSize = .7f;
	offsetY = offset;
	randomisedShape = rand;

}

//
///////////Block's Methods/////////////////
//

void Block::drawBlock(float colour1, float colour2, float colour3)
{
	if (isExisting)
	{
		glPushMatrix();
		glTranslatef(position[0], position[1], 0.0);
		//draw square
		solidSquare(boxSize, colour1, colour2, colour3);
		glPopMatrix();
	}

}

//
//solidSquare
//
//function that uses size, and colours to draw square
void Block::solidSquare(float size, float colour1, float colour2, float colour3)
{
	// set the color for the rectangle
	glColor3f(colour1, colour2, colour3);

	// draw a rectangle with corners at ( .25, .25, 0 ) and ( .75, .75, 0 )
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(size, 0.0, 0.0);
	glVertex3f(size, size, 0.0);
	glVertex3f(0.0, size, 0.0);
	glEnd();
}


//
///////////Grid's Methods/////////////////
//

void Grid::reset()
{
	colourCounter = 0;
	gap = .1f;
	margin = .5f;
	tableSize = CONST_TABLESIZE;
	boxSize = (ORTHOHEIGHT - gap * (CONST_TABLESIZE + 1) - margin * 2) / CONST_TABLESIZE;
	isClicked = false;
	clickedWoodB = 100;
	offsetY = 0;
	isPlaced = false;
	isDisabled = false;
	score = 0;
	numOfWoodBlock = 0;
	for (int j = 0; j < tableSize; j++) //columns
	{

		for (int i = 0; i < tableSize; i++) //rows
		{

			gridBlocks[j][i] = Block();
			gridBlocks[j][i].isExisting = true;
		}
	}
}

void Grid::initGrid()
{
	//reset timer
	gShowTime = 0;

	float yvalCheck, xvalCheck;


	//draw table
	for (int j = 0; j < tableSize; j++) //rows
	{
		//y translate
		yvalCheck = j * boxSize + gap * (j + 1) + margin;

		for (int i = 0; i < tableSize; i++) //columns
		{
			//x translate
			xvalCheck = i * boxSize + gap * (i + 1) + margin;

			gridBlocks[j][i] = Block(xvalCheck, yvalCheck, boxSize);
			gridBlocks[j][i].isExisting = true;
			//make boxes and store their position in an array
			//gridBlocks[j][i] = Block (xvalCheck, yvalCheck);
			//cout << gridBlocks[j][i].position[0];
		}
	}

}

void Grid::drawGrid()
{
	for (int j = 0; j < tableSize; j++) //rows
	{

		for (int i = 0; i < tableSize; i++) //columns
		{
			gridBlocks[j][i].drawBlock(gridBlocks[j][i].colour[0], gridBlocks[j][i].colour[1], gridBlocks[j][i].colour[2]);
		}
	}
}

void Grid::checkClick()
{
	float savedPos = 0;

	vector <int> savedRow;
	vector <int> savedCol;
	vector <int> filledRow;
	vector <int> filledCol;
	if (isDisabled)
	{
		return;
	}

	for (int j = 0; j < tableSize; j++) //rows
	{

		for (int i = 0; i < tableSize; i++) //columns
		{
			if (gridBlocks[j][i].isHovered /*&&
				MySettings.nMousex >= margin + gap &&
				MySettings.nMousex <= gridBlocks[0][4].position[0] + boxSize &&
				MySettings.nMousey >= margin + gap &&
				MySettings.nMousey <= gridBlocks[4][4].position[1] + boxSize*/)
			{
				cout << "yes";
				gridBlocks[j][i].isOccupied = true;
				MyWoodBlock[clickedWoodB]->isPlaced = true;

				if (!checkDuplicate(savedRow, j))
				{
					savedRow.push_back(j);
				}
				if (!checkDuplicate(savedCol, i))
				{
					savedCol.push_back(i);
				}

					gridBlocks[j][i].colour[0] = 0;
					gridBlocks[j][i].colour[1] = 1;
			}
		}
	}

	if (MyWoodBlock[clickedWoodB]->isPlaced)
	{
		int counterX = 0;
		for (unsigned int k = 0; k < savedRow.size(); k++)
		{
			counterX = 0;
			for (int l = 0; l < tableSize; l++)
			{
				if (gridBlocks[savedRow[k]][l].isOccupied)
					counterX++;
			}

			if (counterX == tableSize)
			{
				filledRow.push_back(savedRow[k]);
				cout << counterX;
			}

		}

		int counterY = 0;
		for (unsigned int k = 0; k < savedCol.size(); k++)
		{
			counterY = 0;
			for (int l = 0; l < tableSize; l++)
			{
				if (gridBlocks[l][savedCol[k]].isOccupied)
					counterY++;
			}
			if (counterY == tableSize)
			{
				filledCol.push_back(savedCol[k]);
				cout << counterY;
			}

		}


		cout << " \n";
		for (unsigned int k = 0; k < filledRow.size(); k++)
		{
			cout << "row" << k << ": " << filledRow[k] << ", ";
		}
		cout << " \n";
		for (unsigned int l = 0; l < filledCol.size(); l++)
		{
			cout << "col" << l << ": " << filledCol[l] << ", ";
		}


	}

	savedCol.clear();
	savedRow.clear();


	if (filledCol.size() + filledRow.size() == 0 && MyWoodBlock[clickedWoodB]->isPlaced)
	{
		//srand(time(0));
		gShowTime = 0;
		savedPos = MyWoodBlock[clickedWoodB]->offsetY;
		score += numOfWoodBlock;
		MyWoodBlock.erase(MyWoodBlock.begin() + clickedWoodB);
		MyWoodBlock.push_back(new WoodBlock(1 + rand() % 3, savedPos, rand() % 4));

	}
	else if (filledRow.size() + filledCol.size() >= 1 && filledCol.size() + filledRow.size() <= 6 &&
		MyWoodBlock[clickedWoodB]->isPlaced)
	{

		if (filledCol.size() + filledRow.size() == 1)
			score += 10;
		if (filledCol.size() + filledRow.size() == 2)
			score += 25;
		if (filledCol.size() + filledRow.size() == 3)
			score += 50;
		if (filledCol.size() + filledRow.size() == 4)
			score += 75;
		if (filledCol.size() + filledRow.size() == 5)
			score += 100;
		if (filledCol.size() + filledRow.size() == 6)
			score += 120;


		if (filledCol.size() == 0)
		{
			for (unsigned int l = 0; l < filledRow.size(); l++)
			{
				for (int k = 0; k < tableSize; k++)
				{
					gridBlocks[filledRow[l]][k].isOccupied = false;
					gridBlocks[filledRow[l]][k].colour[0] = 1;
					gridBlocks[filledRow[l]][k].colour[1] = 0;
				}
			}
			filledRow.clear();
			filledCol.clear();
		}
		else if (filledRow.size() == 0)
		{
			for (unsigned int l = 0; l < filledCol.size(); l++)
			{
				for (int k = 0; k < tableSize; k++)
				{
					gridBlocks[k][filledCol[l]].isOccupied = false;
					gridBlocks[k][filledCol[l]].colour[0] = 1;
					gridBlocks[k][filledCol[l]].colour[1] = 0;
				}

			}
			filledRow.clear();
			filledCol.clear();
		}
		else if (filledRow.size() >= 1 && filledCol.size() >= 1)
		{
			for (unsigned int l = 0; l < filledCol.size(); l++)
			{
				for (int k = 0; k < tableSize; k++)
				{
					gridBlocks[k][filledCol[l]].isOccupied = false;
					gridBlocks[k][filledCol[l]].colour[0] = 1;
					gridBlocks[k][filledCol[l]].colour[1] = 0;
				}
			}

			for (unsigned int l = 0; l < filledRow.size(); l++)
			{
				for (int k = 0; k < tableSize; k++)
				{
					gridBlocks[filledRow[l]][k].isOccupied = false;
					gridBlocks[filledRow[l]][k].colour[0] = 1;
					gridBlocks[filledRow[l]][k].colour[1] = 0;
				}
			}
			filledRow.clear();
			filledCol.clear();
		}
		gShowTime = 0;
		filledRow.clear();
		filledCol.clear();
	}
	else if (!MyWoodBlock[clickedWoodB]->isPlaced)
	{
		return;
	}

}

bool checkDuplicate(vector <int> v, int num)
{
	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (v[i] == num)
			return true;
		else
			return false;
	}

}

void Grid::updateGrid()
{

}

void Grid::computeIntersection()
{
	pivot.clear();
	colourCounter = 0;
	tableSize = CONST_TABLESIZE;
	numOfWoodBlock = 4;
	clickedWoodB = 100;

	if (isDisabled)
	{
		return;
	}

	for (unsigned int i = 0; i < MyWoodBlock.size(); i++)
	{
		if (MyWoodBlock[i]->isClicked)
		{
			numOfWoodBlock = MyWoodBlock[i]->getNoOfBlocks();
			clickedWoodB = i;
		}

	}


	for (int j = 0; j < tableSize; j++) //rows
	{

		for (int i = 0; i < tableSize; i++) //columns
		{
			gridBlocks[j][i].isHovered = false;

			if (abs(gridBlocks[j][i].position[0] + .6 - MySettings.nMousex) <= 4 &&
				abs(gridBlocks[j][i].position[1] + .6 - MySettings.nMousey) <= 4)
			{
				for (int k = 0; k < MyWoodBlock[clickedWoodB]->tableSize; k++) //rows
				{

					for (int l = 0; l < MyWoodBlock[clickedWoodB]->tableSize; l++) //columns
					{
						if (MyWoodBlock[clickedWoodB]->gridBlocks[k][l].isExisting &&
							gridBlocks[j][i].isOccupied == false &&
							abs(gridBlocks[j][i].position[0] - MyWoodBlock[clickedWoodB]->getPositionX(k, l)) <= .6 &&
							abs(gridBlocks[j][i].position[1] - MyWoodBlock[clickedWoodB]->getPositionY(k, l)) <= .6)
						{
							if (colourCounter <= numOfWoodBlock)
							{
								//cout << gridBlocks[j][i].isOccupied;
								//cout << numOfWoodBlock;
								pivot.push_back(j);
								pivot.push_back(i);
								gridBlocks[j][i].isHovered = false;
								colourCounter++;

								//gridBlocks[j][i].colour[1] = 1;
								//cout << gridBlocks[j][i].position[1];
								//cout << "\nplacable";
								//cout << colourCounter;

							}
						}
					}
				}
			}
			else
			{
				if (!gridBlocks[j][i].isOccupied)
				{
					gridBlocks[j][i].colour[1] = 0;
				}
				gridBlocks[j][i].isHovered = false;
			}
		}
	}
	//cout << colourCounter;

	if (colourCounter == numOfWoodBlock)
	{
		for (unsigned int p = 0; p < pivot.size(); p += 2)
		{
			gridBlocks[pivot[p]][pivot[p + 1]].colour[1] = .7f;
			gridBlocks[pivot[p]][pivot[p + 1]].isHovered = true;
		}
	}
	else
	{
		for (int j = 0; j < tableSize; j++) //rows
		{

			for (int i = 0; i < tableSize; i++) //columns
			{
				gridBlocks[j][i].isHovered = false;
				if (!gridBlocks[j][i].isOccupied)
				{
					gridBlocks[j][i].colour[1] = 0;
				}

			}
		}
	}

}


float Grid::getPositionX(int j, int i)
{
	return gridBlocks[j][i].position[0];
}

float Grid::getPositionY(int j, int i)
{
	return gridBlocks[j][i].position[1];
}

void Grid::setPositionY(int j, int i, float posY)
{
	gridBlocks[j][i].position[1] = posY;
}

void Grid::setPositionX(int j, int i, float posX)
{
	gridBlocks[j][i].position[0] = posX;
}

int Grid::getNoOfBlocks()
{
	int counter = 0;
	for (int j = 0; j < tableSize; j++) //columns
	{

		for (int i = 0; i < tableSize; i++) //rows
		{
			if (gridBlocks[j][i].isExisting)
				counter++;
		}
	}
	return counter;
}

//
///////////WoodBlock's Methods/////////////////
//

void WoodBlock::initGrid()
{
	boxSize = .7f;
	float yvalCheck, xvalCheck;
	//draw table
	for (int j = 0; j < tableSize; j++) //rows
	{
		//y translate
		yvalCheck = j * boxSize + gap * (j + 1) + margin + offsetY;

		for (int i = 0; i < tableSize; i++) //columns
		{
			//x translate
			xvalCheck = i * boxSize + gap * (i + 1) + margin * 21;
			gridBlocks[j][i] = Block(xvalCheck, yvalCheck, boxSize);
			gridBlocks[j][i].isExisting = true;
			//make boxes and store their position in an array
			//gridBlocks[j][i] = Block (xvalCheck, yvalCheck);
		}
	}
	if (tableSize == 2)
	{
		switch (randomisedShape)
		{
		case 0:
			break;
		case 1:
			gridBlocks[1][1].isExisting = false;
			gridBlocks[1][0].isExisting = false;
			break;
		case 2:
			gridBlocks[1][0].isExisting = false;
			break;
		case 3:
			gridBlocks[1][1].isExisting = false;
			break;
		}

	}
	if (tableSize == 3)
	{
		switch (randomisedShape)
		{
		case 0:
			for (int j = 0; j < 2; j++) //rows
			{
				for (int i = 0; i < 2; i++) //columns
				{
					gridBlocks[j][i].isExisting = false;
				}
			}
			break;
		case 1:
			for (int j = 0; j < 3; j++) //rows
			{
				for (int i = 0; i < 3; i++) //columns
				{
					if (j == 1)
						continue;
					gridBlocks[j][i].isExisting = false;
				}
			}
			break;
		case 2:
			for (int j = 0; j < 3; j++) //rows
			{
				for (int i = 0; i < 3; i++) //columns
				{
					if (j == 1)
						continue;
					gridBlocks[j][i].isExisting = false;
				}
			}
			gridBlocks[2][2].isExisting = true;
			break;
		case 3:
			break;
		}


	}
}

void WoodBlock::drawGrid()
{
	for (int j = 0; j < tableSize; j++) //columns
	{

		for (int i = 0; i < tableSize; i++) //rows
		{
			gridBlocks[j][i].drawBlock(colour[0], colour[1], colour[2]);
		}
	}

	//glPushMatrix();
	//glTranslatef(gridBlocks[0][1].position[0], gridBlocks[0][1].position[1], 0.0);
	//glutSolidSphere(.6, 20, 20);
	//glPopMatrix();

}

void WoodBlock::checkClick()
{
	if (isDisabled)
	{
		return;
	}
	//cout << endl << gridBlocks[0][1].position[0] << endl << gridBlocks[0][1].position[1];
	//cout << endl << gridBlocks[0][0].position[0] << endl << gridBlocks[0][0].position[0] + boxSize;
	if (MySettings.nMousex >= gridBlocks[0][0].position[0] &&
		MySettings.nMousex <= gridBlocks[0][tableSize - 1].position[0] + boxSize &&
		MySettings.nMousey >= gridBlocks[0][0].position[1] &&
		MySettings.nMousey <= gridBlocks[tableSize - 1][0].position[1] + boxSize)
	{
		isClicked = true;

	}
	else
	{
		isClicked = false;
	}

}

void WoodBlock::updateGrid()
{

	//cout << tableSize;
	float yvalCheck, xvalCheck;
	for (int j = 0; j < tableSize; j++) //columns
	{

		for (int i = 0; i < tableSize; i++) //rows
		{
			gridBlocks[j][i].boxSize = (ORTHOHEIGHT - gap * (CONST_TABLESIZE + 1) - margin * 2) / CONST_TABLESIZE;
			if (MySettings.nMousex > 14)
			{
				yvalCheck = (j - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (j + 1) + MySettings.nMousey;
				xvalCheck = (i - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (i + 1) + 14.01f;

				if (MySettings.nMousey < margin || MySettings.nMousey > ORTHOHEIGHT - margin)
				{
					return;
				}
			}
			else if (MySettings.nMousex < margin)
			{
				yvalCheck = (j - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (j + 1) + MySettings.nMousey;
				xvalCheck = (i - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (i + 1) + margin - .01f;

				if (MySettings.nMousey < margin || MySettings.nMousey > ORTHOHEIGHT - margin)
				{
					return;
				}
			}
			else if (MySettings.nMousey < margin)
			{
				xvalCheck = (i - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (i + 1) + MySettings.nMousex;
				yvalCheck = (j - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (j + 1) + margin - .01f;

				if (MySettings.nMousex > 14 || MySettings.nMousex < margin)
				{
					return;
				}
			}
			else if (MySettings.nMousey > ORTHOHEIGHT - margin)
			{
				xvalCheck = (i - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (i + 1) + MySettings.nMousex;
				yvalCheck = (j - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (j + 1) + ORTHOHEIGHT - margin + 0.1f;
				if (MySettings.nMousex > 14 || MySettings.nMousex < margin)
				{
					return;
				}
			}
			else
			{
				xvalCheck = (i - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (i + 1) + MySettings.nMousex;
				yvalCheck = (j - (float)tableSize / 2) * gridBlocks[j][i].boxSize + gap * (j + 1) + MySettings.nMousey;
			}


			gridBlocks[j][i].position[0] = xvalCheck;
			gridBlocks[j][i].position[1] = yvalCheck;


		}
	}
	//cout << boxSize;
}

