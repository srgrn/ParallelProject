#pragma once
#include "pointxy.h"
#include <string>

class ControlPoint :
	public PointXY
{
public:
	ControlPoint(void);
	ControlPoint(double x,double y,char* strtime);
	~ControlPoint(void);
	int timeInSeconds; // in order to make it easier i will define time as seconds from midnight
	int timeToInt(int hour,int min,int sec);
};

