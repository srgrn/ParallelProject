/******************************************
 * Class for hodling a plane object
 ******************************************/

#pragma once
#include "ControlPoint.h"
#include "PointXY.h" // since controlPoint is a kind of PointXY this is not actually needed
#include "Cell.h"
#include "ProjectSpace.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Plane
{
public:
	Plane(void);
	Plane(istream& is); // get plane from data file
	~Plane(void);
	
	PointXY calculateDirectionVector();
	void updateLocation(int interval,ProjectSpace *space);
	void step(int time, int interval,ProjectSpace *space);
	bool isMoving();
	bool isMoving(int time);

	int flightNumber;
	ControlPoint location;
	PointXY direction;
	vector<ControlPoint> controlpoints;
	Cell *currentCell;
	int CD; // critical Degree
	map<int,bool> CDObjects; //critical degree objects
	// operators
	bool Plane::operator>(const Plane& other) const;
	bool Plane::operator<(const Plane& other) const;
	bool Plane::operator==(const Plane& other) const;
	bool Plane::operator>=(const Plane& other) const;
	bool Plane::operator<=(const Plane& other) const;
};

