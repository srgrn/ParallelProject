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
	bool isZero();
};

