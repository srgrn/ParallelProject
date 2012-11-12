#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "board.h"

using namespace std;

class helpers
{
public:
	helpers(void);
	~helpers(void);
	board readHeader(istream& is);
	fstream openFile(char *filename);
	bool checkGridLineX(double m,double n,int y,int start,int end);
	bool checkGridLineY(double m,double n,int x,int start,int end);
};

