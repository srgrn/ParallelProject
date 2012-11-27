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
ControlPoint::ControlPoint(double x,double y,int time):PointXY(x,y)
{
	timeInSeconds = time;
}

ControlPoint::~ControlPoint(void)
{
}

int ControlPoint::timeToInt(int hour,int min,int sec)
{
	// using simple math (each hour has 3600 seconds and each minute 60 seconds
	return (hour*3600 + min*60 + sec);
}

int* ControlPoint::toArray()
{
	int *ret = (int*)malloc(sizeof(int) * 3);
	ret[0] = x;
	ret[1] = y;
	ret[2] = timeInSeconds;
	return ret;
}
void ControlPoint::print()
{
	printf("Control Point: %.0f,%.0f time %d\n",x,y,timeInSeconds);
}