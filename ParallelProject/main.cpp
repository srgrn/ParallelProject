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
#include <mpi.h>
using namespace std;

#define DAYINSECONDS 86400
#define NUM_THREADS 4
#define TAG_REQWORK 70
#define TAG_ENDWORK 71
#define TAG_MOREWORK 72
#define TAG_UPDATE 80
#define TAG_UPDATESIZE 81
#define TAG_SENDUPDATE 82



#define MAGIC_NUMBER 4 //used for cutting time into smaller pieces
void setPairs(vector<Plane*>* vec,pair<Plane*,Plane*> *arr); // this bit of code is creating pairs of stuff

void main(int argc, char* argv[]) { 
	// simplest usage of MPICH as possible just like in the homework
	int  namelen, numprocs, id;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);	
	MPI_Get_processor_name(processor_name,&namelen);
	MPI_Status status;
	omp_set_num_threads(NUM_THREADS);		
	
	map<pair<PointXY,PointXY> ,ViewPath> paths;
	int times[2] = {0}; // buffer for times
	if(id==0)  // load from file on first computer
	{ 

		// create the ProjectSpace from file and load the planes
		fstream stream;
		string str = "C:\\data\\data2.txt";
		//stream.open(argv[1],ios::in);
		stream.open(str,ios::in);
		ProjectSpace space(stream); // read the header of the file # note there are no exception handling.
		map<int,Plane> planes;
		while(stream.good()) // read all planes into memory
		{	 
			Plane temp(stream);
			planes.insert(pair<int,Plane>(temp.flightNumber,temp));
		}
		int timeshare = DAYINSECONDS/(numprocs-1)/MAGIC_NUMBER;
		int* spaceArr = space.toArray();
		MPI_Bcast(spaceArr,5,MPI_INT,0,MPI_COMM_WORLD); // send board data
		free(spaceArr);
		int numOfPlanes = planes.size();
		MPI_Bcast(&numOfPlanes,1,MPI_INT,0,MPI_COMM_WORLD); // sned number of planes
		for(map<int,Plane>::iterator it = planes.begin();it != planes.end();it++)
		{
			int *plane = it->second.toArray();
			int sendSize=plane[1]*3+2;
			MPI_Bcast(&sendSize,1,MPI_INT,0,MPI_COMM_WORLD); // send plane buffer size
			MPI_Bcast(plane,sendSize,MPI_INT,0,MPI_COMM_WORLD); // send plane data
			free(plane);
		}
		int finished = 1;
		while(finished < numprocs)
		{
		int recvBufferSize = 0;
		MPI_Recv(&recvBufferSize,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status); 
		
		if(status.MPI_TAG != TAG_REQWORK)// having some update
			{
				int source = status.MPI_SOURCE;
				MPI_Recv(&recvBufferSize,1,MPI_INT,source,TAG_UPDATE,MPI_COMM_WORLD,&status); // get amount of updates
				int count = recvBufferSize;
				for(int i=0;i<count;i++)
				{
					MPI_Recv(&recvBufferSize,1,MPI_INT,source,TAG_UPDATESIZE,MPI_COMM_WORLD,&status); 
					int *arr = (int*)malloc(sizeof(int)*recvBufferSize); // get single plane update
					MPI_Recv(arr,recvBufferSize,MPI_INT,source,TAG_UPDATE,MPI_COMM_WORLD,&status);
					planes.find(arr[0])->second.updatePlane(arr+1); // update relevant plane
					free(arr);
				}
				//cout << finished<<" master finished updated data from "  << source<<endl;
			}
			else //request for time share
			{
				if(times[1] < DAYINSECONDS)
				{
				times[1] +=timeshare; 
				MPI_Send(times,2,MPI_INT,status.MPI_SOURCE,TAG_MOREWORK,MPI_COMM_WORLD); // send start and end time
				times[0]+=timeshare;
				}
				else // no more time in the day sending quit tag
				{
					MPI_Send(&recvBufferSize,1,MPI_INT,status.MPI_SOURCE,TAG_ENDWORK,MPI_COMM_WORLD);
					finished++;
				}
			}
		}
		Plane *MaxCL =&planes.begin()->second; //take the first plane as first
		for(map<int,Plane>::iterator iter = planes.begin();iter != planes.end();iter++)
		{
			if(MaxCL->CD < iter->second.CD)
			{
				MaxCL = &(iter->second);
				//cout << "flight: " << MaxCL->flightNumber << " CD: " <<MaxCL->CD<<endl;
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
	else // slave asks for work
	{
		vector<Plane> planes;
		int times[2] = {0};
		int *spaceArr = (int*)malloc(sizeof(int)*5);
		MPI_Bcast(spaceArr,5,MPI_INT,0,MPI_COMM_WORLD); // get board
		ProjectSpace space(spaceArr);
		free(spaceArr);
		//space.print(); // test print
		int numOfPlanes =0;
		MPI_Bcast(&numOfPlanes,1,MPI_INT,0,MPI_COMM_WORLD); // get number of planes
		for(int i=0;i<numOfPlanes;i++)
		{
			int sendSize =0;
			MPI_Bcast(&sendSize,1,MPI_INT,0,MPI_COMM_WORLD); // send plane buffer size
			int *plane = (int*)malloc(sizeof(int) * sendSize);
			MPI_Bcast(plane,sendSize,MPI_INT,0,MPI_COMM_WORLD); // send plane data
			planes.push_back(Plane(plane));
			free(plane);
			//planes.back().print(); //test print
		}
		int one =1;
		MPI_Send(&one,1,MPI_INT,0,TAG_REQWORK,MPI_COMM_WORLD);// request time piece 
		MPI_Recv(times,2,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);// slave start time and slave end time
		while(status.MPI_TAG != TAG_ENDWORK)
		{
			//cout << id <<": starting times " << times[0] << endl;
			for(int i=times[0];i<times[1];i++)
			{
				vector<Plane*> moving; 
#pragma omp parallel
				{
#pragma omp for schedule(dynamic) // becouse the plane might not be moving at all
					for(vector<Plane>::iterator iter = planes.begin();iter < planes.end();iter++)
					{
						if(iter->isMoving(i))
						{
							int ret = iter->step(i,&space);
							
							if(ret == 0)
							{
								moving.push_back(&(*iter));
							}
						}
						else 
						{
							//cout<< id << ": plane " << iter->flightNumber << "not moving at " << i <<endl;
							//starttime = getLower(iter->controlpoints[0].timeInSeconds,starttime);
						}
					}
				}
				int num_of_planes = moving.size();
				int pairs_number = (num_of_planes*(num_of_planes-1))/2;
				pair<Plane*,Plane*> *arr = new pair<Plane*,Plane*>[pairs_number]; // the number of pairs should be n!/k!(n-k)! or in his case (n-1)*n/2
				if(num_of_planes > 0)
				{
					setPairs(&moving,arr);
					//starttime=0;
				}
#pragma omp parallel
				{
#pragma omp for schedule(dynamic) // becouse there is different amount of cells in each one
					for(int j=0;j<pairs_number;j++)
					{
						Cell* a = arr[j].first->currentCell;
						Cell* b = arr[j].second->currentCell;
						pair<PointXY,PointXY> key;
						if(a->TopLeft >= b->TopLeft) // make sure key always be from the higher point to the lower one
						{
							key.first = a->TopLeft;
							key.second = b->TopLeft;
						}
						else
						{
							key.first = b->TopLeft;
							key.second = a->TopLeft;
						} 
						map<pair<PointXY,PointXY>,ViewPath>::iterator index = paths.find(key); // simple optimizaion tries to see if path already exists in memory
						ViewPath* ViewPtr = NULL;
						if(index == paths.end()) // path is not in memory
						{
							vector<Cell*> curr;
							space.betweenTwoPoints(a,b,&curr); // calculate new path
							ViewPath temp(a,b);
							if(curr.size() > 0)
							{
								temp.cells.assign(curr.begin(),curr.end());
							}
							paths.insert(pair<pair<PointXY,PointXY>,ViewPath>(key,temp)); // add to memory
							ViewPtr = &paths.at(key); // this wastes some time but makes sure that the path is in memory
						}
						else
						{
							ViewPtr = &(index->second); // get the cells from the past calculated path
						}
#pragma omp for			// not sure it actually helps in this case the internet says it will but i'm not sure
						for(vector<Cell*>::iterator testerIT = ViewPtr->cells.begin();testerIT<ViewPtr->cells.end();testerIT++)
						{
							if(!(*testerIT)->isEmpty()) // only if cell is not empty
							{
								if((*testerIT) != arr[j].first->currentCell && (*testerIT) != arr[j].second->currentCell) // to prevent the case where the last cell on path is the cell of the last plane
								{
									// plane first cannot see plane second
									arr[j].first->CD++; //update Critical Degree add 1 level
									arr[j].first->CDObjects.insert(pair<int,bool>(arr[j].second->flightNumber,true)); // first plane cannot see the second
									arr[j].second->CDObjects.insert(pair<int,bool>(arr[j].first->flightNumber,true)); // second plane cannot see the first
									arr[j].second->CD++; //update Critical Degree add 1 level for second plane
									break;
								}
							}
						}// end of loop going over cells on path
					}// end of updating planes CD
				}
				//i+=starttime; // stupid optimization to jump i into the next time a planes move if no plane is moving at the moment
			} // end of day look
			int count = planes.size();
			MPI_Ssend(&one,1,MPI_INT,0,TAG_SENDUPDATE,MPI_COMM_WORLD); // using Ssend to make the slave wait until it starts sending becouse some how when not waiting is puts all messages in the buffer togather if they are small enough.
			
			MPI_Ssend(&count,1,MPI_INT,0,TAG_UPDATE,MPI_COMM_WORLD);// amount of planes to update
			for(vector<Plane>::iterator it = planes.begin();it != planes.end();it++)  
			{
				int *msg = it->updatePlaneMessage();
				int bufferSize = 3+msg[2]; // size of the update message (a bit arcane but couldn't think of a better way)
				MPI_Ssend(&bufferSize,1,MPI_INT,0,TAG_UPDATESIZE,MPI_COMM_WORLD);
				MPI_Send(msg,bufferSize,MPI_INT,0,TAG_UPDATE,MPI_COMM_WORLD); // send the msg in the buffer
				free(msg);
				it->resetPlane(); // reset plane to be ready for next time share
			}
			MPI_Send(&one,1,MPI_INT,0,TAG_REQWORK,MPI_COMM_WORLD);// request time piece 
			MPI_Recv(times,2,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);// slave start time and slave end time
		}//end of slave while more work
	
		//cout << id << ": done" << endl;

	}// end of slave and master if
	MPI_Finalize();
}

void setPairs(vector<Plane*>* vec,pair<Plane*,Plane*> *arr) // works on the simple assumption that rotating the second iterator while advancing the first will generate only unique pairs 
{
#pragma omp parallel
	{
		vector<Plane*>::iterator front = vec->begin();
		int counter =0;
#pragma omp for
		for(;front!=vec->end();front++)
		{
#pragma omp for
			for(vector<Plane*>::iterator rotate = front+1;rotate!=vec->end();rotate++)
			{
				arr[counter].first = (*front);
				arr[counter].second = (*rotate);
				counter++;
			}
		}
	}
}