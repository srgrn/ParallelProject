#include "CriticalPath.h"


CriticalPath::CriticalPath(PointXY start, PointXY end,vector<cell> *list)
{
	a = start;
	b = end;
	cellsOnPath.clear();
	cellsOnPath.assign(list->begin(),list->end());
}


CriticalPath::~CriticalPath(void)
{
}
