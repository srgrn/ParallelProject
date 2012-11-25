#include "ControlPoint.h"


ControlPoint::ControlPoint(void)
{
}
ControlPoint::ControlPoint(double x,double y,char* strtime):PointXY(x,y)
{
	int hour,min,sec; // note that removal of the ctime cunstruct means no verifications on the time
	sscanf_s(strtime, "%d:%d:%d", &hour, &min, &sec);
	timeInSeconds = this->timeToInt(hour,min,sec);
}

ControlPoint::~ControlPoint(void)
{
}

int ControlPoint::timeToInt(int hour,int min,int sec)
{
	// using simple math (each hour has 3600 seconds and each minute 60 seconds
	return (hour*3600 + min*60 + sec);
}