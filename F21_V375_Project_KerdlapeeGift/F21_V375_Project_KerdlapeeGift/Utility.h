#pragma once
const int ORTHOHEIGHT = 10;

class Utility
{
public:

	int windowWidth;
	int windowHeight;
	float remapx;
	float remapy;
	float nMousex, nMousey;
	float aspect;
	bool isVertical;

	Utility();
	void doGraphicsInit();
	void doCamera();
	void initWindow();
	void doCameraOrthographic(int w, int h);
	void mousebutton(int button, int state, int x, int y);
	void mousebuttonPos( int x, int y);
	void createWoodBlocks();
};