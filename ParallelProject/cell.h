#pragma once


#include <string>
#include <iostream>
#include <fstream>
#include "PointXY.h"
using namespace std;

class cell
{
public:
	cell(int x,int y,int len,int wid);
	~cell(void);
	bool isInCell(PointXY pt); //check if a point is in the cell
	PointXY coord; //top left corner of the cell
	int length; // on x
	int width; // on y
	void print();
};

