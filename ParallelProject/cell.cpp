#include "cell.h"


cell::cell(int x,int y,int len,int wid)
{
	coord.x = x;
	coord.y = y;
	length = len;
	width = wid;
}

bool cell::isInCell(PointXY pt)
{
	
	if(	(coord.x <= pt.x) 
		&& (coord.y<= pt.y) 
		&& ((coord.x + length) >= pt.x) 
		&& ((coord.y + width) >= pt.y) )
		{
			return true;
		}
	return false;
}

cell::~cell(void)
{
}
