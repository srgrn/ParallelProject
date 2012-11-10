/*
A plane object
*/
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "PointWithTime.h"

using namespace std;

class plane
{
public:
	plane(void);
	plane(istream& is); // constractur from file.
	~plane(void);
	int criticalDegree;
	PointWithTime location; // current location of plane after takeoff
	PointXY direction; // vector of movement (not a real vector since it is already multiplied by speed
	PointXY calculateDirectionVector(); // for calculating the speed and direction
	void updateLocation(int interval); // updating the current location according to the time that passed since last time
	void print();
	int flightNumber; // the name of the plane
	int numOfCheckpoints; // number of points in the plane track can be removed and use the vector.size instead.
	vector<PointWithTime> checkpoints; // the list of the points 
	int criticalLevel;

};

