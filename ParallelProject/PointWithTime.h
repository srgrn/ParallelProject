#pragma once
#include "pointxy.h"
#include <ctime>
#include <string>
using namespace std;

class PointWithTime :
	public PointXY
{
public:
	PointWithTime(void);
	PointWithTime(int x,int y,char* strtime);
	~PointWithTime(void);
	tm ptime; 
};

