
#include "board.h"
#include "Plane.h"
#include "CriticalPath.h"
#include "OrderedSet.h"
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
	int num_of_planes = planes.size();
	int pairs_number = (num_of_planes*(num_of_planes-1))/2;
	OrderedSet *arr = new OrderedSet[pairs_number]; // the number of pairs should be n!/k!(n-k)! or in his case (n-1)*n/2
	brd.setPairs(&planes,arr);
	for(int i=0;i<DAYINSECONDS;i+=brd.sampleInterval) //main loop check at interval
	{
		for(vector<plane>::iterator iter=planes.begin();iter != planes.end();iter++) // foreach plane loop (should be parallelzyed
		{
			brd.setDirtyCell(iter->location,true);
			iter->step(i,brd.sampleInterval);
			brd.setDirtyCell(iter->location,false);
		}
		for(int i=0;i<pairs_number;i++)
		{
			if(!arr[i].first()->direction.isZero() && !arr[i].second()->direction.isZero())
			{
			CriticalPath temp(arr[i].first()->location,arr[i].second()->location,&brd.betweenTwoPoints(arr[i].first()->location,arr[i].second()->location));
			int hiddenobjects =0;
			for(vector<cell>::iterator it = temp.cellsOnPath.begin();it<temp.cellsOnPath.end();it++)
			{
				if(it->dirty_bit>0)
				{
					hiddenobjects++;
				}
			}
			arr[i].first()->criticalDegree += hiddenobjects;
			arr[i].second()->criticalDegree += hiddenobjects;
			}
		}

	}
	return 0;
}

