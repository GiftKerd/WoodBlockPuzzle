#pragma once

#include <vector>
#include <iostream>

using namespace std;

const int CONST_TABLESIZE = 5;

class Block
{
public:
	float boxSize;
	float position[2];
	float colour[3];
	bool isExisting;
	bool isOccupied;
	bool isHovered;

	Block();
	Block(float posX, float posY, float size);
	void drawBlock(float colour1, float colour2, float colour3);
	void solidSquare(float size, float colour1, float colour2, float colour3);

};

class Grid : public Block
{
public:
	Block gridBlocks[CONST_TABLESIZE][CONST_TABLESIZE];
	int colourCounter;
	float gap;
	float margin;
	int tableSize;
	bool isClicked;
	int clickedWoodB;
	vector<int> pivot;
	float offsetY;
	bool isPlaced;
	int score;
	int numOfWoodBlock;
	bool isDisabled;

	Grid();
	Grid(float size);
	virtual void initGrid();
	virtual void drawGrid();
	virtual void checkClick();
	virtual void updateGrid();
	void computeIntersection();
	float getPositionX(int j, int i);
	float getPositionY(int j, int i);
	void setPositionY(int j, int i, float posY);
	void setPositionX(int j, int i, float posX);
	int getNoOfBlocks();
	void reset();
	//vector <Block> gBlocksStorage;
};

class WoodBlock : public Grid
{
public:
	//Block gridBlocks[2][2];
	int randomisedShape;

	WoodBlock();
	WoodBlock(int tablesize,float offset,int rand);
	void initGrid();
	void drawGrid();
	void checkClick();
	void updateGrid();

};
