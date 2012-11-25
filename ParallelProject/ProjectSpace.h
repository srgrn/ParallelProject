#pragma once
#include <vector>
#include <map>
#include "Cell.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class ProjectSpace
{
public:
	ProjectSpace(void);
	ProjectSpace(int length,int width,int numOfCellsH,int numOfSCellsV,int interval);
	ProjectSpace(istream& is);
	~ProjectSpace(void);
	
	vector<Cell*> betweenTwoPoints(PointXY a, PointXY b); 
	void betweenTwoPoints(Cell* begin,Cell* end,vector<Cell*>* ret);
	bool checkGridLineX(double m,double n,int y,int start,int end);
	bool checkGridLineY(double m,double n,int x,int start,int end);
	Cell* insertCell(PointXY point);
	pair<PointXY,PointXY>* makekey(Cell* a, Cell* b);

	int cell_width;
	int cell_height;
	int interval;
	int numCellsX;
	int numCellsY;
	map<PointXY,Cell*> cells;
};

