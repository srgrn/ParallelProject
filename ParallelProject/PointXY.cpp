#include "PointXY.h"

PointXY::PointXY(void)
{
	x=-1;
	y=-1;
}

PointXY::PointXY(double x,double y)
{
	this->x = x;
	this->y = y;
}


PointXY::~PointXY(void)
{
}
PointXY PointXY::operator+( const PointXY& other ) const
{
	return PointXY(x+other.x,y+other.y);
}
PointXY PointXY::operator+( const double& other ) const
{
	return PointXY(x+other,y+other);
}
PointXY PointXY::operator*( const double& other ) const
{
	return PointXY(x*other,y*other);
}

bool PointXY::operator>=(const PointXY& other ) const
{
	if(x>=other.x && y>=other.y)
		return true;
	return false;
}
bool PointXY::operator<=(const PointXY& other ) const
{
	if(x<=other.x && y<=other.y)
		return true;
	return false;
}
// if the points are a,b and c,d decide who is bigger using the distance from 0,0
bool PointXY::operator>(const PointXY& other ) const
{
	double first = x*x+y*y;
	double second = other.x*other.x+other.y*other.y;
	if(first > second)
		return true;
	return false;
}
bool PointXY::operator<(const PointXY& other ) const
{
	double first = x*x+y*y;
	double second = other.x*other.x+other.y*other.y;
	if(first < second)
		return true;
	return false;
}
// verify the point is 0,0
bool PointXY::isZero()
{
	if(x ==0 && y ==0)
		return true;
	return false;
}