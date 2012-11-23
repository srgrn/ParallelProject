#pragma once
#include "ControlPoint.h"
#include "PointXY.h" // since controlPoint is a kind of PointXY this is not actually needed
#include <vector>
using namespace std;

class Plane
{
public:
	Plane(void);
	Plane(istream& is);
	~Plane(void);
	
	PointXY calculateDirectionVector();
	void updateLocation(int interval);
	void step(int time, int interval);
	bool isMoving();
	int flightNumber;
	ControlPoint location;
	PointXY direction;
	vector<ControlPoint> controlpoints;

	// operators
	bool Plane::operator>(const Plane& other) const;
	bool Plane::operator<(const Plane& other) const;
	bool Plane::operator==(const Plane& other) const;
	bool Plane::operator>=(const Plane& other) const;
	bool Plane::operator<=(const Plane& other) const;
};

