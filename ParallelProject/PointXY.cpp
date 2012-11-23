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

bool PointXY::isZero()
{
	if(x ==0 && y ==0)
		return true;
	return false;
}