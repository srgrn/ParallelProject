/******************************************
 * Class for hodling a point object (x and y only)
 ******************************************/

#pragma once
class PointXY
{
public:
	PointXY(void);
	PointXY(double x,double y);
	~PointXY(void);
	double x;
	double y;
	PointXY operator+( const PointXY& other ) const; 
	PointXY operator+( const double& other ) const; 
	PointXY operator*( const double& other ) const; 
	bool operator>=(const PointXY& other ) const;
	bool operator<=(const PointXY& other ) const;
	bool operator>(const PointXY& other ) const; // for some reason those are needed for a map
	bool operator<(const PointXY& other ) const; // for some reason those are needed for a map
	bool isZero(); // simply tests if the point is (0,0)
};

