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
};

