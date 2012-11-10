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
		is.get(str,50,' '); // get the x value
		int x = atoi(str);
		is.get(); // get the space after the x value
		is.get(str,50,' '); // get the Y value
		int y = atoi(str);
		is.getline(str,50); // get the rest of the line (only the timestamp left including the newline
		PointWithTime temp(x,y,str);
		checkpoints.push_back(temp); // enter new checkpoint into plane
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

PointXY plane::calculateDirectionVector()
{
	double vecX = checkpoints.front().x - location.x;
	double vecY = checkpoints.front().y - location.y;
	int seconds = checkpoints.front().timeInt - location.timeInt; // time to go between the first and second checkpoints
	PointXY p(vecX,vecY);
	double lengthX = sqrt(vecX*vecX); // required for setting the speed
	double lengthY = sqrt(vecY*vecY);
	double length = sqrt(vecX*vecX + vecY*vecY);
	p.x = (p.x/length)*(lengthX/(double)seconds); // calculating the direction + speed on x
	p.y = p.y/length*(lengthY/(double)seconds); // calculationg the direction + speed on y
	
	return p;
}
void plane::updateLocation(int interval)
{
	location.timeInt += interval;
	location.x += direction.x*interval;
	location.y += direction.y*interval;
}