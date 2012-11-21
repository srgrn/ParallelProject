
#include "board.h"
#include "Plane.h"
#include "CriticalPath.h"
using namespace std;
#define DAYINSECONDS 86400


int main(int argc, char* argv[]) { 

	// taken from the helpers method to remove this class as unneded
	// open this file
	fstream stream;
	stream.open(argv[1],ios::in);
	board brd(stream); // read the header of the file # note there are no exception handling.
	vector<plane> planes;
	while(stream.good()) // read all planes into memory
	{	 
		planes.push_back(*(new plane(stream)));
	}
	for(int i=0;i<DAYINSECONDS;i+=brd.sampleInterval) //main loop check at interval
	{
		for(vector<plane>::iterator iter=planes.begin();iter != planes.end();iter++) // foreach plane loop (should be parallelzyed
		{
			brd.setDirtyCell(iter->location,true);
			iter->step(i,brd.sampleInterval);
			brd.setDirtyCell(iter->location,false);
		}

	}
	return 0;
}

