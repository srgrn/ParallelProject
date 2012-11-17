/*
The Board class is holing the project spafce and function related to it. 
currently takes long to build since it creates so many cells. 
*/

#pragma once
#include <vector>

#include "cell.h"
using namespace std;

class board
{
public:
	board(int length,int width,int numOfCellsH,int numOfSCellsV,int interval);
	~board(void);
	int sampleInterval; // the lost samle interval
	vector<vector<cell>> matrix; // proably unneded
	vector<cell> betweenTwoPoints(PointXY a, PointXY b);
private:
	int cells_h; // x length of cells
	int cells_v; // y length of cells 
	bool checkGridLineX(double m,double n,int y,int start,int end);
	bool checkGridLineY(double m,double n,int x,int start,int end);
};

