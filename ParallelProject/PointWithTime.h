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

