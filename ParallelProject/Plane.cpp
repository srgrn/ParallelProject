#include "Plane.h"


Plane::Plane(void)
{
	direction.x =0;
	direction.y =0;
	currentCell = NULL;
	location.x = -1;
	location.y = -1;
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
	direction.x =0;
	direction.y =0;
	currentCell = NULL;
	location.x = -1;
	location.y = -1;
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

void Plane::updateLocation(int interval,ProjectSpace *space)
{
	location.timeInSeconds += interval;
	location.x += direction.x*interval;
	location.y += direction.y*interval;
	// the main idea is that a plane can only be in one cell at a time
	if(currentCell == NULL)
		currentCell = space->insertCell(location);
	else if(!currentCell->inCell(location)) // the plane is in the same cell he was before
	{
		currentCell->leave(flightNumber);
		currentCell = space->insertCell(location);
		currentCell->occupy(flightNumber);
	}
}

void Plane::step(int time, int interval,ProjectSpace *space)
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
				cout << flightNumber << " starts at " << time << endl;
				direction = calculateDirectionVector();
				updateLocation(0,space); // to make sure the plane sets the cell in the first place
			}
			else
			{
				cout << flightNumber << " finished at " << time << " " <<endl;
				//cout << time << " " << flightNumber << " " << location.x << "," << location.y << endl;
				direction.x = 0; // reset direction vector
				direction.y = 0; // reset direction vector
			}

		}
		else
		{
			if(!direction.isZero())
			{

				updateLocation(interval,space);
				//cout << time << " " << flightNumber << " " << location.x << "," << location.y << endl;
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
bool Plane::isMoving(int time)
{
	if(isMoving()) // this is used to shorten the call since if the plane is already moving no need to check further
		return true;
	if(controlpoints.size()==0)
		return false;
	int liftoff = controlpoints.front().timeInSeconds;
	int touchdown = controlpoints.back().timeInSeconds;
	if(liftoff <= time && touchdown >= time) //the plane is on a leg
		return true;
	return false;
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