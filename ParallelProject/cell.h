#pragma once

#include "PointXY.h"

class cell
{
public:
	cell(int x,int y,int len,int wid);
	~cell(void);
	bool isInCell(PointXY pt); //check if a point is in the cell
private:
	PointXY coord; //top left corner of the cell
	int length; // on x
	int width; // on y
};

