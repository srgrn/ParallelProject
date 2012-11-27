#include "Plane.h"


Plane::Plane(void)
{
	resetPlane();
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
	resetPlane();
}

Plane::Plane(int* arr)
{
	flightNumber = arr[0];
	int j=0;
	for(int i=0;i<arr[1];i++)
	{
		ControlPoint temp(arr[i+2+j],arr[i+3+j],arr[i+4+j]);
		controlpoints.push_back(temp);
		j+=2;
	}
	resetPlane();

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

int Plane::step(int time, ProjectSpace *space)
{
	if(controlpoints.size() == 0 || time >= controlpoints.back().timeInSeconds)
	{
		direction.x = -1;
		direction.y = -1;
		return 1;
	}
	vector<ControlPoint>::iterator cpi = controlpoints.end();
	for(vector<ControlPoint>::iterator cpi_loop = controlpoints.begin();cpi_loop != controlpoints.end(); cpi_loop++)
	{
		if(time > cpi_loop->timeInSeconds)
		{
			location = *(cpi_loop);
			cpi = cpi_loop;
		}
	}
	int timediff = 0;
	if(cpi != controlpoints.end())
	{
		timediff = time - location.timeInSeconds;
		past_controlPoints.insert(past_controlPoints.begin(),controlpoints.begin(),cpi);
		vector<ControlPoint>::iterator afterErase = controlpoints.erase(controlpoints.begin(),cpi+1); //doesn't include the one pointed by last
		if(afterErase != controlpoints.end()) // if no more points for this plane
			{
				//cout << flightNumber << " starts at " << time << endl;
				direction = calculateDirectionVector();
				updateLocation(timediff,space); // to make sure the plane sets the cell in the first place
			}
			else
			{
				//cout << flightNumber << " finished at " << time << " " <<endl;
				//cout << time << " " << flightNumber << " " << location.x << "," << location.y << endl;
				direction.x = 0; // reset direction vector
				direction.y = 0; // reset direction vector
			}
	}
	else
	{
		if(controlpoints.size() == 0)
			return 1;
		if(!direction.isZero())
			{
				updateLocation(timediff,space);
				//cout << time << " " << flightNumber << " " << location.x << "," << location.y << endl;
			}
	}
	return 0;
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
				//cout << flightNumber << " starts at " << time << endl;
				direction = calculateDirectionVector();
				updateLocation(0,space); // to make sure the plane sets the cell in the first place
			}
			else
			{
				//cout << flightNumber << " finished at " << time << " " <<endl;
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
	if(controlpoints.size()==0)
		return false;
	int liftoff = controlpoints.front().timeInSeconds;
	int touchdown = controlpoints.back().timeInSeconds;
	if(liftoff < time && touchdown > time) //the plane is on a leg
		return true;
	if(isMoving()) // this is used to shorten the call since if the plane is already moving no need to check further
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

int* Plane::toArray()
{
	int* ret = (int*)malloc(sizeof(int) * (2+controlpoints.size()*3));
	ret[0] = flightNumber;
	ret[1] = controlpoints.size();
	int pointsStart=0;
	for(int i=0;i<ret[1];i++)
	{
		int *point = controlpoints.at(i).toArray();
		ret[i+2+pointsStart] = point[0];
		ret[i+3+pointsStart] = point[1];
		ret[i+4+pointsStart] = point[2];
		pointsStart+=2;
	}
	return ret;
}
int* Plane::updatePlaneMessage()
{
	int* ret = (int*)malloc(sizeof(int) * (3+CDObjects.size()));
	ret[0] = flightNumber;
	ret[1] = CD;
	ret[2] = CDObjects.size();
	int i=3;
	for(map<int,bool>::iterator it = CDObjects.begin(); it != CDObjects.end(); it++,i++)
	{
		ret[i] = it->first;
	}
	return ret;
}
void Plane::updatePlane(int* arr) // should get the array from the CD location
{
	CD += arr[0];
	for(int i=2;i<(arr[1]+2);i++)
	{
		CDObjects.insert(pair<int,bool>(arr[i],true));
	}
}
void Plane::print()
{
	printf("FlightNumber %d; number of points %d\n",flightNumber,controlpoints.size());
	for(vector<ControlPoint>::iterator it = controlpoints.begin(); it!= controlpoints.end();it++)
	{
		it->print();
	}
	printf("Degree = %d CDObjects: ",CD);
	for(map<int,bool>::iterator it = CDObjects.begin(); it != CDObjects.end();it++)
	{
		printf(" %d  ",it->first);
	}
	printf("\n");
}
void Plane::resetPlane()
{
	direction.x =0;
	direction.y =0;
	currentCell = NULL;
	location.x = -1;
	location.y = -1;
	CD = 0;
	if(past_controlPoints.size() > 0)
	{
		controlpoints.insert(controlpoints.begin(),past_controlPoints.begin(),past_controlPoints.end());
		past_controlPoints.clear();
	}
}