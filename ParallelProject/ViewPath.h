/******************************************
 * Class for holding a path object
 ******************************************/
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

	vector<Cell*> cells; // pointers to all the cells on the Project space between the two points
	Cell* start; // first point on line
	Cell* end; // last point on line
};

