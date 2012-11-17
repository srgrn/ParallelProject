#include "board.h"


board::board(int length,int width,int numOfCellsH,int numOfSCellsV,int interval)
{
	cells_h = length / numOfCellsH;
	cells_v = width / numOfSCellsV;
	sampleInterval = interval;
	
	for(int i=0;i<numOfCellsH;i++)
	{
		vector<cell> temp;
		for(int j=0;j<numOfSCellsV;j++)
		{
			cell c(i*cells_h,j*cells_v,cells_v,cells_h);
			temp.push_back(c);
		}
		matrix.push_back(temp);
	}
}

vector<cell> board::betweenTwoPoints(PointXY a, PointXY b)
{
	vector<cell> ret;
	cell t = matrix[(int)(a.x/cells_h)][(int)(a.y/cells_v)];
	double xdiff = a.x- b.x;
	double ydiff = a.y- b.y;
	double m = (a.y-b.y)/(a.x-b.x);// the curve?
	double n = b.y-m*b.x;// from y=mx+n
	ret.push_back(t);
	cell end = matrix[(int)(b.x/cells_h)][(int)(b.y/cells_v)];
	do
	{
		PointXY temp = t.coord;
		if(xdiff > 0)
		{
		
			if(ydiff==0 && t.coord.x >= end.coord.x)
			{
				t.coord.x -= cells_h;
				ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}
			else if(checkGridLineY(m,n,t.coord.x,t.coord.y,t.coord.y+cells_v))
			{
				t.coord.x -= cells_h;
				ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}

		}
		else if(xdiff <0)
		{
			if(ydiff==0 && t.coord.x <= end.coord.x)
			{			
				t.coord.x += cells_h;
				ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}
			else if(checkGridLineY(m,n,t.coord.x+cells_h,t.coord.y,t.coord.y+cells_v))
			{
				t.coord.x += cells_h;
				ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}
		}
		if(ydiff > 0)
		{
			if(xdiff==0 && t.coord.y > end.coord.y)
			{
				t.coord.y -= cells_v;
				ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}
			else if(checkGridLineX(m,n,t.coord.y,t.coord.x,t.coord.x+cells_h))
			{
				t.coord.y -= cells_v;
				ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}
		}
		else if(ydiff <0)
		{
			if(xdiff==0 && t.coord.y < end.coord.y)
			{
				t.coord.y += cells_v;
				ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}
			else if(checkGridLineX(m,n,t.coord.y+cells_v,t.coord.x,t.coord.x+cells_h))
			{
				t.coord.y += cells_v;
				ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(t.coord.y/cells_v)]);
			}
		}
	} while (t.coord.x != end.coord.x || t.coord.y != end.coord.y);
	return ret;
}

bool board::checkGridLineX(double m,double n,int y,int start,int end)
{
	double temp = (y-n)/m;
	if(temp>=start && temp<=end)
	{
		return true;
	}
	return false;
}
bool board::checkGridLineY(double m,double n,int x,int start,int end)
{
	double temp = m*x+n;
	if(temp>=start && temp<=end)
	{
		return true;
	}
	return false;
}

board::~board(void)
{
}
