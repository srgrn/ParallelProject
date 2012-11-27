/******************************************
 * Class for hodling a point with a timestamp object
 ******************************************/
#pragma once
#include "pointxy.h"
#include <string>

class ControlPoint :
	public PointXY
{
public:
	ControlPoint(void);
	ControlPoint(double x,double y,char* strtime);
	ControlPoint(double x,double y,int time);
	~ControlPoint(void);
	int timeInSeconds; // in order to make it easier i will define time as seconds from midnight
	int timeToInt(int hour,int min,int sec); // convert time string (hh:mm:ss) to seconds from midnight
	int* toArray();// a very basic seralization function
	void print();
};

