#pragma once
#include "cell.h"
class board
{
public:
	board(int length,int width,int numOfCellsH,int numOfSCellsV,int interval);
	~board(void);
	int sampleInterval;
	cell **matrix;
private:
	int cells_h;
	int cells_v;
};

