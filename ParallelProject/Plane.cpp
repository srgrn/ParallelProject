#include "Plane.h"


Plane::Plane(void)
{
}

Plane::Plane(istream& is)
{
	char str[100];
	is.getline(str,100);
	flightNumber = atoi(str);
	is.getline(str,50);
	int numOfcontrolPoints = atoi(str);
	for(int i=0;i<numOfcontrolPoints;i++)
	{
		is.get(str,50,' '); // get the x value
		int x = atoi(str);
		is.get(); // get the space after the x value
		is.get(str,50,' '); // get the Y value
		int y = atoi(str);
		is.getline(str,50); // get the rest of the line (only the timestamp left including the newline
		ControlPoint temp(x,y,str);
		controlpoints.push_back(temp); // enter new checkpoint into plane
	}
}

Plane::~Plane(void)
{
}

PointXY Plane::calculateDirectionVector()
{
	double vecX = controlpoints.front().x - location.x;
	double vecY = controlpoints.front().y - location.y;
	int seconds = controlpoints.front().timeInSeconds - location.timeInSeconds; // time to go between the first and second controlPoints
	PointXY p(vecX,vecY);
	double lengthX = sqrt(vecX*vecX); // required for setting the speed
	double lengthY = sqrt(vecY*vecY);
	double length = sqrt(vecX*vecX + vecY*vecY);
	p.x = (p.x/length)*(lengthX/(double)seconds); // calculating the direction + speed on x
	p.y = p.y/length*(lengthY/(double)seconds); // calculationg the direction + speed on y

	return p;
}

void Plane::updateLocation(int interval)
{
	location.timeInSeconds += interval;
	location.x += direction.x*interval;
	location.y += direction.y*interval;
}

void Plane::step(int time, int interval)
{
	if(controlpoints.size() > 0)
	{
		vector<ControlPoint>::iterator cpi = controlpoints.begin();
		if(cpi->timeInSeconds == time) // if started on a control point 
		{

			location = *(cpi);
			vector<ControlPoint>::iterator afterErase = controlpoints.erase(cpi);
			if(afterErase != controlpoints.end()) // if no more points for this plane
			{
				//cout << flightNumber << " starts at " << time << endl;
				direction = calculateDirectionVector();
			}
			else
			{
				//cout << flightNumber << " finished at " << time << " ";
				//cout << time << " " << flightNumber << " " << location.x << "," << location.y << endl;
				direction.x = 0; // reset direction vector
				direction.y = 0; // reset direction vector
			}

		}
		else
		{
			if(!direction.isZero())
			{

				updateLocation(interval);
				//cout << i << " " << iter->flightNumber << " " << iter->location.x << "," << iter->location.y << endl;
			}
		}
	}

}

bool Plane::isMoving()
{
	if(direction.isZero())
		return false;
	return true;
}

// Operators
bool Plane::operator>(const Plane& other) const
{
	if(flightNumber > other.flightNumber)
		return true;
	return false;
}

bool Plane::operator<(const Plane& other) const
{
	if(flightNumber < other.flightNumber)
		return true;
	return false;
}

bool Plane::operator==(const Plane& other) const
{
	if(flightNumber == other.flightNumber)
		return true;
	return false;
}

bool Plane::operator>=(const Plane& other) const
{
	if(flightNumber > other.flightNumber || flightNumber== other.flightNumber)
		return true;
	return false;
}

bool Plane::operator<=(const Plane& other) const
{
	if(flightNumber < other.flightNumber || flightNumber== other.flightNumber)
		return true;
	return false;
}