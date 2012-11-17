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
	
	double temp = (b.x-a.x);
	double m = (b.y - a.y)/temp;
	double n = b.y-m*b.x;
	if(temp == 0)
	{
		m=0;
	}
	
	int firstcellx = a.x/cells_h;
	int firstcelly = a.y/cells_v;
	ret.push_back(matrix[firstcellx][firstcelly]);
	
	int lastcellx = b.x/cells_h;
	int lastcelly = b.y/cells_v;
	//ret.push_back(matrix[lastcellx][lastcelly]);
	double xdiff = b.x -a.x;
	double ydiff = b.y -a.y;
	
	cell t(firstcellx*cells_h,firstcelly*cells_v,cells_h,cells_v);
	while (t.coord.x != lastcellx*cells_h || t.coord.y != lastcelly*cells_v)
	{
		PointXY nextCell = t.coord;
		bool testx = false;
		bool testy = false;
			// deciding on which sides of the cell to test default values are top and right sides
		if(xdiff > 0) // test left side
		{
			nextCell.x += cells_h;
			testx = checkGridLineY(m,n,nextCell.x,t.coord.y,t.coord.y+cells_v);
		}
		if(ydiff > 0) // test bottom side
		{
			nextCell.y += cells_v;
			testy = checkGridLineX(m,n,nextCell.y,t.coord.x,t.coord.x+cells_h);
		}
				 
		if(testx)
		{
			ret.push_back(matrix[(int)(nextCell.x/cells_h)][(int)(t.coord.y/cells_v)]);
		}
		if(testy)
		{
			ret.push_back(matrix[(int)(t.coord.x/cells_h)][(int)(nextCell.y/cells_v)]);
		}
		if(testx && testy)
		{
			ret.push_back(matrix[(int)(nextCell.x/cells_h)][(int)(nextCell.y/cells_v)]);
		}
		t.coord.x = nextCell.x;
		t.coord.y = nextCell.y;

	}
	
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
