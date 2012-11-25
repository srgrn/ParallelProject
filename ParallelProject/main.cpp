#pragma once
#include "ProjectSpace.h"
#include "Cell.h"
#include "Plane.h"
#include "ViewPath.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


#define DAYINSECONDS 86400

void setPairs(vector<Plane*>* vec,pair<Plane*,Plane*> *arr); // this bit of code is creating pairs of stuff

void main(int argc, char* argv[]) { 
	
	// create the ProjectSpace from file and load the planes
	fstream stream;
	stream.open(argv[1],ios::in);
	ProjectSpace space(stream); // read the header of the file # note there are no exception handling.
	vector<Plane> planes;
	map<pair<PointXY,PointXY> ,ViewPath*> paths;
	while(stream.good()) // read all planes into memory
	{	 
		planes.push_back(*(new Plane(stream)));
	}
	//TODO transmit data here
	for(int i=0;i<DAYINSECONDS;i++)
	{
		vector<Plane*> moving; 
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
		setPairs(&moving,arr);
		for(int j=0;j<num_of_planes;j++)
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
			map<pair<PointXY,PointXY>,ViewPath*>::iterator index = paths.find(key);
			if(index == paths.end())
			{
				ViewPath curr(a,b);
				space.betweenTwoPoints(a,b,&(curr.cells));
				index = paths.insert(pair<pair<PointXY,PointXY>,ViewPath*>(key,&curr)).first;
			}
			vector<Cell*>* ptr = &(index->second->cells);
			for(vector<Cell*>::iterator testerIT = ptr->begin();testerIT<ptr->end();testerIT++)
			{
				if((*testerIT)->contents.size() >0)
				{
					arr[j].first->CL++;
					//arr[j].first->CDObjects.insert
					arr[j].second->CL++;
					break;
				}
			}
			
		}

	}
}


void setPairs(vector<Plane*>* vec,pair<Plane*,Plane*> *arr)
{
	vector<Plane*>::iterator front = vec->begin();
	int counter =0;
	for(;front<vec->end();front++)
	{
		for(vector<Plane*>::iterator rotate = front;rotate<vec->end();rotate++,counter++)
		{
			arr[counter].first = (*front);
			arr[counter].second = (*rotate);
		}
	}
}
