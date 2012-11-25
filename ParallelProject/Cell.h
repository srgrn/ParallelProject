/******************************************
 * Class for hodling a cell object
 ******************************************/
#pragma once
#include "PointXY.h"
#include <map>
using namespace std;

class Cell
{
public:
	Cell(void);
	Cell(int x,int y,int xsize,int ysize);
	~Cell(void);
	//PointXY center; //can be added to simplyfy calculations
	PointXY TopLeft;
	int width;
	int height;
	PointXY center();
	map<int,bool> contents; // holds the flightnumbers of the planes inside
	bool isEmpty(); // return true if the cell is empty
	bool occupy(int id); // add to contents
	bool leave(int id); // remove from contents 
	bool inCell(PointXY point); // Helper function not used


};

