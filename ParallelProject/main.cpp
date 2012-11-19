
#include "helpers.h"
#include "Plane.h"
#include "CriticalPath.h"
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
	for(int i=0;i<86400;i+=brd.sampleInterval) //main loop check at interval
	{
		for(vector<plane>::iterator iter=planes.begin();iter != planes.end();iter++) // foreach plane loop (should be parallelzyed
		{
			brd.setDirtyCell(iter->location,true);
			iter->step(i,brd.sampleInterval);
			brd.setDirtyCell(iter->location,false);
		}
		vector<CriticalPath> critic;
		//CriticalPath trek(planes[0].location,planes[2].location,brd.betweenTwoPoints(planes.front().location,planes[2].location));
	}
	return 0;
}

