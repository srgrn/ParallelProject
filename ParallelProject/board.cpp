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
	double m = (b.y - a.y)/(b.x-a.x);
	double n = b.y-m*b.x;
	int firstcellx = a.x/cells_h;
	int firstcelly = a.y/cells_v;
	ret.push_back(matrix[firstcellx][firstcelly]);
	int lastcellx = b.x/cells_h;
	int lastcelly = b.y/cells_v;
	ret.push_back(matrix[lastcellx][lastcelly]);
	return ret;
}

board::~board(void)
{
}
