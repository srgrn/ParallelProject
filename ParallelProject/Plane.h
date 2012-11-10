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
	void print();
private:
	int flightNumber;
	int numOfCheckpoints;
	vector<PointWithTime> checkpoints;
	int criticalLevel;

};

