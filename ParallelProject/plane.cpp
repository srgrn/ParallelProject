#include "plane.h"


plane::plane(void)
{
}
plane::plane(istream& is)
{
	char str[100];
	is.getline(str,100);
	flightNumber = atoi(str);
	is.getline(str,50);
	numOfCheckpoints = atoi(str);
	criticalDegree = 0;
	criticalLevel = 0;
	for(int i=0;i<numOfCheckpoints;i++)
	{
		is.get(str,50,' ');
		int x = atoi(str);
		is.get();
		is.get(str,50,' ');
		int y = atoi(str);
		PointXY temp(x,y);
		checkpoints.push_back(temp);
		is.getline(str,100);
		// TODO handle time of checkpoint
	}
}

plane::~plane(void)
{
}
