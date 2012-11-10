#pragma once

#include "PointXY.h"

class cell
{
public:
	cell(int x,int y,int len,int wid);
	~cell(void);
	bool isInCell(PointXY pt);
private:
	PointXY coord;
	int length;
	int width;
};

