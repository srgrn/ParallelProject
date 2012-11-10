/*
this class in the same as a pointXY but with added time valriable and is used to represent a point in time.
*/
#pragma once
#include "pointxy.h"
#include <ctime>

#include <string>
using namespace std;

class PointWithTime :
	public PointXY
{
public:
	PointWithTime(void);
	PointWithTime(double x,double y,char* strtime);
	int timeToInt();
	~PointWithTime(void);
	tm ptime; 
	int timeInt;
};

