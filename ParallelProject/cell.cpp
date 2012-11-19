#include "cell.h"


cell::cell(int x,int y,int len,int wid)
{
	coord.x = x;
	coord.y = y;
	length = len;
	width = wid;
	dirty_bit = 0;
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

void cell::print()
{
	cout <<"Cell (top,left)=(" << coord.x << "," << coord.y << ")" << endl;
}
void cell::clean()
{
	dirty_bit--;
}
void cell::dirty()
{
	dirty_bit++;
}

cell::~cell(void)
{
}
