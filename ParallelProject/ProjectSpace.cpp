#include "ProjectSpace.h"


ProjectSpace::ProjectSpace(void)
{
}

ProjectSpace::ProjectSpace(int xsize,int ysize,int numOfCellsX,int numOfSCellsY,int sec) // constructor from info
{
	cell_width = xsize / numOfCellsX;
	cell_height = ysize / numOfSCellsY;
	numCellsX = numOfCellsX; 
	numCellsY = numOfSCellsY; 
	interval = sec;
}
ProjectSpace::ProjectSpace(int* arr) // constructor from arr
{
	cell_width = arr[0];
	cell_height = arr[1];
	numCellsX = arr[2]; 
	numCellsY = arr[3]; 
	interval = arr[4];
}

ProjectSpace::ProjectSpace(istream& is) // constructur from file
{
	char str[50];
	vector<int> params; // setting a vector for the header 
	while(is.good() && params.size()<=4) // the header paramaters are very specificaly designated
	{	 
		is.getline(str,50); // get line
		params.push_back(atoi(str)); // convert to number
	}
	cell_width = params[0] / params[2]; // the first woud be total size in meters for X the third is the number of cells on x
	cell_height = params[1] / params[3]; // the second woud be total size in meters for Y the fourth is the number of cells on Y
	numCellsX = params[2]; // total number of cells on X
	numCellsY = params[3]; // total number of cells on Y
	interval = params[4]; 
}

ProjectSpace::~ProjectSpace(void)
{
}

vector<Cell*> ProjectSpace::betweenTwoPoints(PointXY a, PointXY b) //DECREPATED
{
	vector<Cell*> ret;
	return ret;
}
// there is a much better algorithm called Siboon algorithm but it is a bit complicated and i couldn't understand it.
void ProjectSpace::betweenTwoPoints(Cell* begin,Cell* end,vector<Cell*>* ret)
{
	if(begin == end) // if the cells recieved are on the same cell the function will return
		return;
	Cell start; // temp cell for moving
	// setting the cell
	start.TopLeft = begin->TopLeft; 
	start.width = begin->width; 
	start.height = begin->height;

	double xdiff = start.center().x- end->center().x; // the diff between the start cell and the end cell on X
	double ydiff = start.center().y- end->center().y; // the diff between the start cell and the end cell on Y
	
	// calculating y=mx+n
	double m = (start.center().y-end->center().y)/(start.center().x-end->center().x);
	double n = end->center().y-m*end->center().x;// from y=mx+n
	
	int breakloop = (int)sqrt((double)(numCellsX*numCellsX+numCellsY*numCellsY)) * 2; // the max of cells on route due to a bug in my algorithm (not the best but will be removed later
	do
	{
		PointXY temp = start.TopLeft;
		if(xdiff > 0) // going left
		{
			if(ydiff==0 && start.TopLeft.x >= end->TopLeft.x) // parallel to X
			{
				temp.x -= cell_width;
			}
			else if(checkGridLineY(m,n,start.TopLeft.x,start.TopLeft.y,start.TopLeft.y+cell_height)) // check if i'm going thorugh left side
			{
				temp.x -= cell_width;
				ret->push_back(insertCell(PointXY(temp.x,start.TopLeft.y))); // add the cell to the list
			}

		}
		else if(xdiff <0) // going right
		{
			if(ydiff==0 && start.TopLeft.x <= end->TopLeft.x) // parallel to X
			{			
				temp.x += cell_width;
			}
			else if(checkGridLineY(m,n,start.TopLeft.x+cell_width,start.TopLeft.y,start.TopLeft.y+cell_height)) // checking right side
			{
				temp.x += cell_width;
				ret->push_back(insertCell(PointXY(temp.x,start.TopLeft.y)));
			}
		}
		if(ydiff > 0) // going up
		{
			if(xdiff==0 && start.TopLeft.y > end->TopLeft.y) // parallel to X
			{
				temp.y -= cell_height;
			}
			else if(checkGridLineX(m,n,start.TopLeft.y,start.TopLeft.x,start.TopLeft.x+cell_width)) // checking upper side
			{
				temp.y -= cell_height;
				ret->push_back(insertCell(PointXY(start.TopLeft.x,temp.y)));
			}
		}
		else if(ydiff <0) // going down
		{
			if(xdiff==0 && start.TopLeft.y < end->TopLeft.y) // parallel to Y
			{
				temp.y += cell_height;
			}
			else if(checkGridLineX(m,n,start.TopLeft.y+cell_height,start.TopLeft.x,start.TopLeft.x+cell_width)) // checking lower side
			{
				temp.y += cell_height;
				ret->push_back(insertCell(PointXY(start.TopLeft.x,temp.y)));
			}
		}
		// advancing cell
		start.TopLeft.x = temp.x; 
		start.TopLeft.y = temp.y;
		// if i have atleast one cell and it is not the one i'm in now than i passed thorugh a corner and added both left/right a up/down cells and need to add the new one
		if(ret->size() != 0 && (ret->back()->TopLeft.x != start.TopLeft.x || ret->back()->TopLeft.y != start.TopLeft.y)) 
		{
			ret->push_back(insertCell(start.TopLeft));	
		}
		breakloop--; // breaking the loop if i got stuck
	} while ((start.TopLeft.x != end->TopLeft.x || start.TopLeft.y != end->TopLeft.y)&& breakloop >0 ); // finish the loop when reached end cell or breakloop == 0
	return;
}
// simply insert Y value and check resulting X value is in range
bool ProjectSpace::checkGridLineX(double m,double n,int y,int start,int end)
{
	double temp = (y-n)/m;
	if(temp>=start && temp<=end)
	{
		return true;
	}
	return false;
}
// simply insert X value and check resulting Y value is in range
bool ProjectSpace::checkGridLineY(double m,double n,int x,int start,int end)
{
	double temp = m*x+n;
	if(temp>=start && temp<=end)
	{
		return true;
	}
	return false;
}

// add a new cell
Cell* ProjectSpace::insertCell(PointXY point)
{
	// getting top left point of a cell which must be an int
	// using the fact that if i cast the result into int it will be without the leftover
	int x = (int)(point.x/cell_width);
	int y = (int)(point.y/cell_height); 
	x *= cell_width;
	y *= cell_height;
	PointXY curr(x,y);
	map<PointXY,Cell*>::iterator index = cells.find(curr); // verify cell doesn't exist
	if(index != cells.end())
	{
		// cell already exist return it 
		return index->second;
	}
	Cell *ret = new Cell(x,y,cell_width,cell_height);
	cells.insert(pair<PointXY,Cell*>(ret->TopLeft,ret)); // add new cell to Project Space
	return ret; // return new cell pointer
}

// not sure this is the correct place for it.
pair<PointXY,PointXY>* makekey(Cell* a, Cell* b)
{
	// the special case of this make key function is the the pair would be in a way an unordererd pair since
	// the lower point of the two would be the first
	if(a->TopLeft >= b->TopLeft)
		return &pair<PointXY,PointXY>(a->TopLeft,b->TopLeft);
	return &pair<PointXY,PointXY>(b->TopLeft,a->TopLeft);
}

int* ProjectSpace::toArray()
{
	int *ret = (int*)malloc(sizeof(int) *5);
	ret[0] = cell_width;
	ret[1] = cell_height;
	ret[2] = numCellsX;
	ret[3] = numCellsY;
	ret[4] = interval;
	return ret;
}

void ProjectSpace::print()
{
	printf("width = %d, height=%d, num of Cells X = %d, num of Cells Y = %d, interval = %d\n",cell_width,cell_height,numCellsX,numCellsY,interval);
}