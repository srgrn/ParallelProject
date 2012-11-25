#pragma once
class PointXY
{
public:
	PointXY(void);
	PointXY(double x,double y);
	~PointXY(void);
	double x;
	double y;
	PointXY operator+( const PointXY& other ) const; // while this is used
	PointXY operator+( const double& other ) const; // and this is used
	PointXY operator*( const double& other ) const; // and this only to make it oneline instead of two
	bool operator>=(const PointXY& other ) const;
	bool operator<=(const PointXY& other ) const;
	bool operator>(const PointXY& other ) const; // for some reason those are needed for a map
	bool operator<(const PointXY& other ) const; // for some reason those are needed for a map
	bool isZero();
};

