#pragma once
#pragma once
#include "stdafx.h"

#include <string>

using namespace std;

void solidSquare(float size, float colour1, float colour2, float colour3);

class Table
{
public:
	float gap;
	float margin;
	int tableSize;

	double variant;
	float boxSize;
	float xval, yval;
	float colour[3];
	int variantPos[2];
	int colourSwitch;
	bool correct;
	int score;
	int error;

	bool disabled;

	Table();
	void drawTable();
	void initTable();
	void checkClick();
	void reset();
};