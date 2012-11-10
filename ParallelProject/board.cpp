#include "board.h"


board::board(int length,int width,int numOfCellsH,int numOfSCellsV,int interval)
{
	cells_h = length / numOfCellsH;
	cells_v = width / numOfSCellsV;
	sampleInterval = interval;
}


board::~board(void)
{
}
