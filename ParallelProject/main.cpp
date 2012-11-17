
#include "helpers.h"
#include "Plane.h"

using namespace std;



int main(int argc, char* argv[]) { 

	
	helpers h;
	fstream stream = h.openFile(argv[1]);
	board brd = h.readHeader(stream);
	vector<plane> planes;
	while(stream.good())
	{	 
		planes.push_back(*(new plane(stream)));
	}
	PointXY a(50,50);
	PointXY b(50,250);
	vector<cell> occupiedCells = brd.betweenTwoPoints(a,b);
	for(vector<cell>::iterator it = occupiedCells.begin(); it != occupiedCells.end(); it++)
	{
		it->print();
	}
	return 0;
	for(int i=0;i<86400;i+=brd.sampleInterval) //main loop check at interval
	{
		for(vector<plane>::iterator iter=planes.begin();iter != planes.end();iter++) // foreach plane loop (should be parallelzyed
		{
			iter->step(i,brd.sampleInterval);
		}
	}
	return 0;
}

