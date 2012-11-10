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


board::~board(void)
{
}
