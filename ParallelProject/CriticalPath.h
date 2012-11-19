#pragma once
#include "cell.h"
#include "PointXY.h"
#include <vector>
class CriticalPath
{
public:
	CriticalPath(PointXY start, PointXY end,vector<cell> *list);
	~CriticalPath(void);
	PointXY a;
	PointXY b;
	vector<cell> cellsOnPath;
};

