#pragma once
#include "Cell.h"
#include <vector>
using namespace std;

class ViewPath
{
public:
	ViewPath(void);
	ViewPath(Cell* a,Cell* b);
	~ViewPath(void);

	vector<Cell*> cells;
	Cell* start;
	Cell* end;
};

