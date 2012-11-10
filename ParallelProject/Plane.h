#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "PointXY.h"

using namespace std;

class plane
{
public:
	plane(void);
	plane(istream& is);
	~plane(void);
	int criticalDegree;
private:
	int flightNumber;
	int numOfCheckpoints;
	vector<PointXY> checkpoints;
	int criticalLevel;

};

