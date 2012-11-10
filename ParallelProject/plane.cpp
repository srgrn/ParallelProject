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
		is.getline(str,50);
		PointWithTime temp(x,y,str);
		checkpoints.push_back(temp);
	}
}

plane::~plane(void)
{
}
void plane::print()
{
	printf("Flight: %d\nNumber of Points: %d\n",flightNumber,numOfCheckpoints);
	for(vector<PointWithTime>::iterator it= checkpoints.begin();it != checkpoints.end();it++)
	{
		printf("%d,%d - %d:%d:%d\n",it->x,it->y,it->ptime.tm_hour,it->ptime.tm_min,it->ptime.tm_sec);
	}
	printf("Critical Level: %d, Critical Degree: %d\n", criticalLevel,criticalDegree);
}
