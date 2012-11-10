#include "PointWithTime.h"


PointWithTime::PointWithTime(void)
{
}
PointWithTime::PointWithTime(int x,int y,char* strtime):PointXY(x,y)
{
	sscanf_s(strtime, "%d:%d:%d", &ptime.tm_hour, &ptime.tm_min, &ptime.tm_sec);
}


PointWithTime::~PointWithTime(void)
{
}
