#pragma once

#include <vector>

#include "cell.h"
using namespace std;

class board
{
public:
	board(int length,int width,int numOfCellsH,int numOfSCellsV,int interval);
	~board(void);
	int sampleInterval;
	vector<vector<cell>> matrix;
	vector<cell> betweenTwoPoints(PointXY a, PointXY b);
private:
	int cells_h;
	int cells_v;
};

