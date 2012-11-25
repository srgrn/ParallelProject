#include "ProjectSpace.h"


ProjectSpace::ProjectSpace(void)
{
}

ProjectSpace::ProjectSpace(int xsize,int ysize,int numOfCellsX,int numOfSCellsY,int sec)
{
	cell_width = xsize / numOfCellsX;
	cell_height = ysize / numOfSCellsY;
	interval = sec;
}

ProjectSpace::ProjectSpace(istream& is)
{
	char str[50];
	vector<int> params;
	while(is.good() && params.size()<=4)
	{	 
		is.getline(str,50);
		params.push_back(atoi(str));
	}
	cell_width = params[0] / params[2];
	cell_height = params[1] / params[3];
	interval = params[4];
}

ProjectSpace::~ProjectSpace(void)
{
}

vector<Cell*> ProjectSpace::betweenTwoPoints(PointXY a, PointXY b)
{
	vector<Cell*> ret;
	return ret;
}
void ProjectSpace::betweenTwoPoints(Cell* begin,Cell* end,vector<Cell*>* ret)
{
	int a[5] ={0};
	
	//test.push_back(begin);
	//Cell t = matrix[(int)(a.x/cells_h)][(int)(a.y/cells_v)];
	Cell start(begin->TopLeft.x,begin->TopLeft.y,begin->width,begin->height);
	
	if(begin == end)
		return;
	double xdiff = start.center().x- end->center().x;
	double ydiff = start.center().y- end->center().y;
	double m = (start.center().y-end->center().y)/(start.center().x-end->center().x);// the curve?
	double n = end->center().y-m*end->center().x;// from y=mx+n
	ret->push_back(end);
	//Cell end = matrix[(int)(b.x/cells_h)][(int)(b.y/cells_v)];
	do
	{
		PointXY temp = start.TopLeft;
		if(xdiff > 0)
		{
			if(ydiff==0 && start.TopLeft.x >= end->TopLeft.x)
			{
				temp.x -= cell_width;
			}
			else if(checkGridLineY(m,n,start.TopLeft.x,start.TopLeft.y,start.TopLeft.y+cell_height))
			{
				temp.x -= cell_width;
				//TODO new pushing of cells
				ret->push_back(insertCell(PointXY(temp.x,start.TopLeft.y)));
				//ret.push_back(matrix[(int)(temp.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}

		}
		else if(xdiff <0)
		{
			if(ydiff==0 && start.TopLeft.x <= end->TopLeft.x)
			{			
				temp.x += cell_width;
			}
			else if(checkGridLineY(m,n,start.TopLeft.x+cell_width,start.TopLeft.y,start.TopLeft.y+cell_height))
			{
				temp.x += cell_width;
				//TODO push cells into list
				ret->push_back(insertCell(PointXY(temp.x,start.TopLeft.y)));
				//ret.push_back(matrix[(int)(temp.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}
		}
		if(ydiff > 0)
		{
			if(xdiff==0 && start.TopLeft.y > end->TopLeft.y)
			{
				temp.y -= cell_height;
			}
			else if(checkGridLineX(m,n,start.TopLeft.y,start.TopLeft.x,start.TopLeft.x+cell_width))
			{
				temp.y -= cell_height;
				// TODO oush cells into list
				ret->push_back(insertCell(PointXY(start.TopLeft.x,temp.y)));
				//ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(temp.y/cells_v)]);
			}
		}
		else if(ydiff <0)
		{
			if(xdiff==0 && start.TopLeft.y < end->TopLeft.y)
			{
				temp.y += cell_height;
			}
			else if(checkGridLineX(m,n,start.TopLeft.y+cell_height,start.TopLeft.x,start.TopLeft.x+cell_width))
			{
				temp.y += cell_height;
				// TODO push cells into list
				ret->push_back(insertCell(PointXY(start.TopLeft.x,temp.y)));
				//ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(temp.y/cells_v)]);
			}
		}
		start.TopLeft.x = temp.x;
		start.TopLeft.y = temp.y;
		if(ret->back()->TopLeft.x != start.TopLeft.x || ret->back()->TopLeft.y != start.TopLeft.y)
		{
			ret->push_back(insertCell(start.TopLeft));	
		}
	} while (start.TopLeft.x != end->TopLeft.x || start.TopLeft.y != end->TopLeft.y);
	
	return;
}

bool ProjectSpace::checkGridLineX(double m,double n,int y,int start,int end)
{
	double temp = (y-n)/m;
	if(temp>=start && temp<=end)
	{
		return true;
	}
	return false;
}
bool ProjectSpace::checkGridLineY(double m,double n,int x,int start,int end)
{
	double temp = m*x+n;
	if(temp>=start && temp<=end)
	{
		return true;
	}
	return false;
}


Cell* ProjectSpace::insertCell(PointXY point)
{
	// using the fact that if i cast the result into int it will be without the leftover
	int x = (int)(point.x/cell_width);
	int y = (int)(point.y/cell_height); 
	x *= cell_width;
	y *= cell_height;
	PointXY curr(x,y);
	map<PointXY,Cell*>::iterator index = cells.find(curr);
	if(index != cells.end())
	{
		return index->second;
	}
	Cell *ret = new Cell(x,y,cell_width,cell_height);
	cells.insert(pair<PointXY,Cell*>(ret->TopLeft,ret));
	return ret;
}

pair<PointXY,PointXY>* makekey(Cell* a, Cell* b)
{
	if(a->TopLeft >= b->TopLeft)
		return &pair<PointXY,PointXY>(a->TopLeft,b->TopLeft);
	return &pair<PointXY,PointXY>(b->TopLeft,a->TopLeft);
}