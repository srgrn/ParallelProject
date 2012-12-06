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
	Plane(int* arr);
	~Plane(void);
	
	PointXY calculateDirectionVector();
	void updateLocation(int interval,ProjectSpace *space); // updating the plane location and current cell
	void updateLocation(int interval); // updating the plane location and current cell
	int Plane::step(int time, ProjectSpace *space); // moving the plane in space and time according to flight plan
	void step(int time, int interval,ProjectSpace *space); // DEPRECATED
	bool isMoving(); // simply checkes the direction of the plane
	bool isMoving(int time); // verify if a plane is moving in a given time
	int* toArray();// a very basic seralization function
	int* updatePlaneMessage(); // this creates update message of a plane
	void updatePlane(int *arr); // actually updates the plane based on a message
	void print();
	void resetPlane(); // resets the plane paramters back to original values at creation


	int flightNumber;
	ControlPoint location;
	PointXY direction;
	vector<ControlPoint> controlpoints;
	vector<ControlPoint> past_controlPoints;
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

