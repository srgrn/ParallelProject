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
	plane(istream& is);
	~plane(void);
	int criticalDegree;
	PointWithTime location;
	PointXY direction;
	double flightSpeed;
	PointXY calculateDirectionVector();
	void updateLocation(int interval);
	void print();
	int flightNumber;
	int numOfCheckpoints;
	vector<PointWithTime> checkpoints;
	int criticalLevel;

};

