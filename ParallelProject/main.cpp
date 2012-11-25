#pragma once
#include "ProjectSpace.h"
#include "Cell.h"
#include "Plane.h"
#include "ViewPath.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <omp.h>
using namespace std;

#define DAYINSECONDS 86400
#define NUM_THREADS 4

void setPairs(vector<Plane*>* vec,pair<Plane*,Plane*> *arr); // this bit of code is creating pairs of stuff

// the main problem is that i can proably improve this code by writing it better a aserial code than as a parallel code
// sicne i have thoguht of the algorithm first in seriel (even if it does split nicely into taske) it doesn't get much improvments from parallel
// i only have one big loop and 2 small ones inside it.
void main(int argc, char* argv[]) { 
	
	// create the ProjectSpace from file and load the planes
	fstream stream;
	stream.open(argv[1],ios::in);
	ProjectSpace space(stream); // read the header of the file # note there are no exception handling.
	vector<Plane> planes;
	map<pair<PointXY,PointXY> ,ViewPath> paths;
	while(stream.good()) // read all planes into memory
	{	 
		planes.push_back(*(new Plane(stream)));
	}
	//TODO transmit data here
	omp_set_num_threads(NUM_THREADS);		
	for(int i=0;i<DAYINSECONDS;i++)
	{
		vector<Plane*> moving; 

		#pragma omp parallel for
		for(vector<Plane>::iterator iter = planes.begin();iter < planes.end();iter++)
		{
			if(iter->isMoving(i))
			{
				iter->step(i,space.interval,&space);
				moving.push_back(&(*iter));
			}
		}
		int num_of_planes = moving.size();
		int pairs_number = (num_of_planes*(num_of_planes-1))/2;
		pair<Plane*,Plane*> *arr = new pair<Plane*,Plane*>[pairs_number]; // the number of pairs should be n!/k!(n-k)! or in his case (n-1)*n/2
		if(num_of_planes > 0)
		{
		setPairs(&moving,arr); // this function cause the heap corruptions
		}
		//if(num_of_planes > 0)
		//{
		//	cout << "yeah";
		//	// test heap corruption
		//	Cell* a = new Cell(0,0,100,100); 
		//	Cell* b = a;
		//	pair<PointXY,PointXY> key;
		//	if(a->TopLeft >= b->TopLeft)
		//	{
		//		key.first = a->TopLeft;
		//		key.second = b->TopLeft;
		//	}
		//	else
		//	{
		//		key.first = b->TopLeft;
		//		key.second = a->TopLeft;
		//	} 
	
		//	ViewPath curr(a,b);
		//	space.betweenTwoPoints(a,b,&(curr.cells));
		//	map<pair<PointXY,PointXY>,ViewPath*>::iterator index =paths.insert(pair<pair<PointXY,PointXY>,ViewPath*>(key,&curr)).first;
		//	ViewPath* ViewPtr = paths.at(key);
		//	for(vector<Cell*>::iterator testerIT = ViewPtr->cells.begin();testerIT<ViewPtr->cells.end();testerIT++)
		//	{
		//		cout << (*testerIT)->isEmpty() <<endl;
		//	}
		//	// end of test 
		//}
		#pragma omp for schedule(dynamic)
		for(int j=0;j<pairs_number;j++)
		{
			Cell* a = arr[j].first->currentCell;
			Cell* b = arr[j].second->currentCell;
			pair<PointXY,PointXY> key;
			if(a->TopLeft >= b->TopLeft)
			{
				key.first = a->TopLeft;
				key.second = b->TopLeft;
			}
			else
			{
				key.first = b->TopLeft;
				key.second = a->TopLeft;
			} 
			map<pair<PointXY,PointXY>,ViewPath>::iterator index = paths.find(key);
			ViewPath* ViewPtr = NULL;
			if(index == paths.end())
			{
				
				vector<Cell*> curr;
				space.betweenTwoPoints(a,b,&curr);
				ViewPath temp(a,b);
				if(curr.size() > 0)
				{
				temp.cells.assign(curr.begin(),curr.end());
				}
				paths.insert(pair<pair<PointXY,PointXY>,ViewPath>(key,temp));
				ViewPtr = &paths.at(key); // this adds a lot of O into the code 
			}
			else
			{
				ViewPtr = &(index->second); // this adds a lot of O into the code 
			}
			#pragma omp parallel for			
			for(vector<Cell*>::iterator testerIT = ViewPtr->cells.begin();testerIT<ViewPtr->cells.end();testerIT++)
				{
					if(!(*testerIT)->isEmpty())
					{
						if((*testerIT) != arr[j].first->currentCell && (*testerIT) != arr[j].second->currentCell)
						{
						// plane first cannot see plane second
						arr[j].first->CD++; //update Critical Degree add 1 level
						arr[j].first->CDObjects.insert(pair<int,bool>(arr[j].second->flightNumber,true)); // first plane cannot see the second
						arr[j].second->CDObjects.insert(pair<int,bool>(arr[j].first->flightNumber,true)); // second plane cannot see the first
						arr[j].second->CD++; //update Critical Degree add 1 level for second plane
						break;
						}
					}
				} // end of loop going over cells on path
		} // end of updating planes CD
	} // end of day look
	
	Plane *MaxCL =&planes.front();
	for(vector<Plane>::iterator iter = planes.begin();iter < planes.end();iter++)
	{
		if(MaxCL->CD < iter->CD)
		{
			MaxCL = &(*iter);
		}
	}
	cout << "flight with max Critical Degree: " << MaxCL->flightNumber << " CD: " <<MaxCL->CD<<" hidden from ";
	#pragma omp parallel for
	for(map<int,bool>::iterator it = MaxCL->CDObjects.begin();it!= MaxCL->CDObjects.end();it++)
	{
		cout << it->first << " ";
	}
	cout << endl;
}


void setPairs(vector<Plane*>* vec,pair<Plane*,Plane*> *arr)
{
	vector<Plane*>::iterator front = vec->begin();
	int counter =0;
	#pragma omp parallel for
	for(;front!=vec->end();front++)
	{
		#pragma omp parallel for
		for(vector<Plane*>::iterator rotate = front+1;rotate!=vec->end();rotate++)
		{
			arr[counter].first = (*front);
			arr[counter].second = (*rotate);
			counter++;
		}
	}
}
