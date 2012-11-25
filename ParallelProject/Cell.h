#pragma once
#include "PointXY.h"
//#include "Plane.h"
#include <map>
using namespace std;

class Cell
{
public:
	Cell(void);
	Cell(int x,int y,int xsize,int ysize);
	~Cell(void);
	//PointXY center; can be added to simplyfy calculations
	PointXY TopLeft;
	int width;
	int height;
	PointXY center();
	//map<int,bool> contents; 
	int contents;
	bool isEmpty();
	bool occupy(int id);
	bool leave(int id);
	bool inCell(PointXY point);


};

