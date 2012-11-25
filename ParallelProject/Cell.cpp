#include "Cell.h"

Cell::Cell(void)
{
}
Cell::Cell(int x,int y,int xsize,int ysize)
{
	TopLeft.x = x;
	TopLeft.y = y;
	width = xsize;
	height = ysize;
}


Cell::~Cell(void)
{
}

PointXY Cell::center()
{
	return PointXY(TopLeft.x+width/2,TopLeft.y+height/2);
}
bool Cell::isEmpty()
{
	//if(contents.size() == 0)
	if(contents ==0)
		return true;
	return false;
}

bool Cell::occupy(int id)
{
	contents++;
	//return (contents.insert(pair<int,bool>(id,true))).second; // which in this case will only return false if the plane is already in the map
	return true;

}

bool Cell::leave(int id)
{
	contents--;
	//int ret = contents.erase(id);
	//if(ret ==0)
		//return false;
	return true;
}

bool Cell::inCell(PointXY point)
{
	if(point>=TopLeft && point<=(TopLeft+100))
		return true;
	return false;
}