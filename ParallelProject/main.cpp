


#include "helpers.h"
#include "Plane.h"

using namespace std;



int main(int argc, char* argv[]) { 

	
	helpers h;
	fstream stream = h.openFile(argv[1]);
	board b = h.readHeader(stream);
	vector<plane> planes;
	while(stream.good())
	{	 
		planes.push_back(*(new plane(stream)));
	}
	for(vector<plane>::iterator it = planes.begin();it != planes.end();it++)
	{
		it->print();
	}
	
	return 0;
}

