/******************************************
* Class for hodling a Project Space object
******************************************/
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
	ProjectSpace(int length,int width,int numOfCellsH,int numOfSCellsV,int interval); // constructor from info (used for testing)
	ProjectSpace(istream& is); // constructur from file header not safe and without exception handling
	ProjectSpace(int* arr); // constructor from array
	~ProjectSpace(void);

	vector<Cell*> betweenTwoPoints(PointXY a, PointXY b); // get a list of all cells between two points // DEPRECATED
	void betweenTwoPoints(Cell* begin,Cell* end,vector<Cell*>* ret); // get a list of all cells between two cells
	bool checkGridLineX(double m,double n,double y,double start,double end); // check if the line given by y=mx+n cuts X=a in a specific cell
	bool checkGridLineY(double m,double n,double x,double start,double end); // check if the line given by y=mx+n cuts Y=a in a specific cell
	Cell* insertCell(PointXY point); // addes new cell to the board
	pair<PointXY,PointXY>* makekey(Cell* a, Cell* b); // create pair from two cells // DEPRECATED
	int* toArray(); // a very basic seralization function
	void print();

	int cell_width; // width of cells on project space (X)
	int cell_height; // height of cells on project space (Y)
	int interval; // time interval for each check
	int numCellsX; // number of max cells on grid on X
	int numCellsY; // number of max cells on grid on Y
	map<PointXY,Cell*> cells; // collection of all cells that have been used already
};

