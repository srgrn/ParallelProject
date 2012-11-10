


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
	PointXY b(150,50);
	brd.betweenTwoPoints(a,b);
	return 0;
	for(int i=0;i<86400;i+=1) //main loop check at interval
	{
		for(vector<plane>::iterator iter=planes.begin();iter != planes.end();iter++) // foreach plane loop (should be parallelzyed
		{
			if(iter->checkpoints.size() > 0)
			{
				vector<PointWithTime>::iterator cpi = iter->checkpoints.begin();
				if(cpi->timeInt == i)
				{
					
					iter->location = *(cpi);
					vector<PointWithTime>::iterator afterErase = iter->checkpoints.erase(cpi);
					if(afterErase != iter->checkpoints.end()) // if no more points for this plane
					{
						cout << iter->flightNumber << " starts at " << i << endl;
						iter->direction = iter->calculateDirectionVector();
					}
					else
					{
						//cout << iter->flightNumber << " finished at " << i << " ";
						cout << i << " " << iter->flightNumber << " " << iter->location.x << "," << iter->location.y << endl;
						iter->direction.x = -1; // reset direction vector
						iter->direction.y = -1; // reset direction vector
					}
					
				}
				else
				{
					if(iter->direction.x != -1)
					{

						iter->updateLocation(1); //TODO change to be interval from data file which have been lost somewhere.
						//cout << i << " " << iter->flightNumber << " " << iter->location.x << "," << iter->location.y << endl;
					}
				}
			}

		}
	}
	return 0;
}

