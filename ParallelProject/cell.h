#pragma once

#include "PointXY.h"

class cell
{
public:
	cell(int x,int y,int len,int wid);
	~cell(void);
private:
	PointXY coord;
	int length;
	int width;
};

