#pragma once
#include "PointXY.h"
#include "Plane.h"
#include <map>
using namespace std;

class Cell
{
public:
	Cell(int x,int y,int xsize,int ysize);
	~Cell(void);
	//PointXY center; can be added to simplyfy calculations
	PointXY TopLeft;
	int width;
	int height;
	PointXY center();
	map<int,Plane*> contents; 
	bool isEmpty();
	bool occupy(Plane* ptr);
	bool leave(Plane* ptr);
};

