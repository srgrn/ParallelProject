#include "PointWithTime.h"


PointWithTime::PointWithTime(void)
{
}
PointWithTime::PointWithTime(double x,double y,char* strtime):PointXY(x,y)
{
	sscanf_s(strtime, "%d:%d:%d", &ptime.tm_hour, &ptime.tm_min, &ptime.tm_sec);
	timeInt = this->timeToInt();
}


PointWithTime::~PointWithTime(void)
{
}

int PointWithTime::timeToInt()
{
	return ptime.tm_hour*3600 + ptime.tm_min*60 + ptime.tm_sec;
}