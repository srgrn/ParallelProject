#include "ParallelProject.h"


ParallelProject::ParallelProject(void)
{
	//	times[0] = 0;
	//	times[1] = 0;

}


ParallelProject::~ParallelProject(void)
{
}

void ParallelProject::setPairs(Plane* planesArr,int numOfPlanes,pair<Plane*,Plane*> *arr) // works on the simple assumption that rotating the second iterator while advancing the first will generate only unique pairs 
{
	int counter =0;
	for(int first = 0;first < numOfPlanes;first++)
	{
		for(int second = second+1;second<numOfPlanes;second++)
		{
			arr[counter].first = &planesArr[first];
			arr[counter].second = &planesArr[second];
			counter++;
		}
	}
}

void ParallelProject::setPairs(vector<Plane*>* vec,pair<Plane*,Plane*> *arr) // works on the simple assumption that rotating the second iterator while advancing the first will generate only unique pairs 
{
	vector<Plane*>::iterator front = vec->begin();
	int counter =0;
	for(;front!=vec->end();front++)
	{
		for(vector<Plane*>::iterator rotate = front+1;rotate!=vec->end();rotate++)
		{
			arr[counter].first = (*front);
			arr[counter].second = (*rotate);
			counter++;
		}
	}
}

void ParallelProject::mpi_init(int argc, char* argv[])
{
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);	
	MPI_Get_processor_name(processor_name,&namelen);

}

void ParallelProject::master()
{
	double startruntime = MPI_Wtime();
	int times[2] = {0};
	// create the ProjectSpace from file and load the planes
	fstream stream;
	string str = "C:\\data\\data3.txt";
	map<int,Plane> planes;
	//stream.open(argv[1],ios::in);
	stream.open(str,ios::in);
	space = ProjectSpace(stream);
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
	MPI_Barrier(MPI_COMM_WORLD);
	int finished = 1;
	cout << numprocs << " " << finished << endl;
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
				cout << "sending " << times[0] << ","<< times[1] << " to " << status.MPI_SOURCE << endl;
				MPI_Ssend(times,2,MPI_INT,status.MPI_SOURCE,TAG_MOREWORK,MPI_COMM_WORLD); // send start and end time
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
	for(map<int,bool>::iterator it = MaxCL->CDObjects.begin();it!= MaxCL->CDObjects.end();it++)
	{
		cout << it->first << " ";
	}
	cout << endl;
	double endruntime = MPI_Wtime();
	MPI_Finalize();
	cout << "total time = " << endruntime-startruntime << endl;
}


void ParallelProject::slave()
{
	int times[2];
	int *spaceArr = (int*)malloc(sizeof(int)*5);
	MPI_Bcast(spaceArr,5,MPI_INT,0,MPI_COMM_WORLD); // get board
	space = ProjectSpace(spaceArr);
	free(spaceArr);
	int numOfPlanes =0;
	MPI_Bcast(&numOfPlanes,1,MPI_INT,0,MPI_COMM_WORLD); // get number of planes
	Plane** planesArr = (Plane**)malloc(sizeof(Plane*) * numOfPlanes);
	for(int i=0;i<numOfPlanes;i++)
	{
		int sendSize =0;
		MPI_Bcast(&sendSize,1,MPI_INT,0,MPI_COMM_WORLD); // send plane buffer size
		int *plane = (int*)malloc(sizeof(int) * sendSize);
		MPI_Bcast(plane,sendSize,MPI_INT,0,MPI_COMM_WORLD); // send plane data
		planesArr[i] = new Plane(plane);
		free(plane);
		//planes.back().print(); //test print
	}
	MPI_Barrier(MPI_COMM_WORLD);
	int one =1;
	MPI_Send(&one,1,MPI_INT,0,TAG_REQWORK,MPI_COMM_WORLD);// request time piece 
	MPI_Recv(times,2,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);// slave start time and slave end time
	map<pair<PointXY,PointXY> ,ViewPath> paths;
	while(status.MPI_TAG != TAG_ENDWORK)
	{
		//cout << id <<": starting times " << times[0] << endl;
		for(int i=times[0];i<times[1];i++)
		{
			vector<Plane*> moving; 
#pragma omp parallel for
			for(int j=0; j< numOfPlanes;j++)
			{
				int tid = omp_get_thread_num();
				if(planesArr[j]->isMoving(i))
				{
					int ret = planesArr[j]->step(i,&space);
					if(ret == 0)
					{
						Cell* ptr = planesArr[j]->currentCell;
						if(ptr == NULL)
						{
#pragma omp critical
							{
								ptr = space.insertCell(planesArr[j]->location);
								ptr->occupy(planesArr[j]->flightNumber);
								planesArr[j]->currentCell = ptr;
							}
						}
						else if(!ptr->inCell(planesArr[j]->location)) // the plane is in the same cell he was before
						{
#pragma omp critical
							{
								ptr->leave(planesArr[j]->flightNumber);
								ptr = space.insertCell(planesArr[j]->location);
								ptr->occupy(planesArr[j]->flightNumber);
								planesArr[j]->currentCell = ptr;
							}
						}
#pragma omp critical
						moving.push_back(planesArr[j]);
					}
				}
			}
int numOfMoving;
int pairs_number;
pair<Plane*,Plane*> *arr;
#pragma omp single 
			{
				numOfMoving = moving.size();
				 pairs_number = (numOfMoving*(numOfMoving-1))/2;
				arr = new pair<Plane*,Plane*>[pairs_number]; // the number of pairs should be n!/k!(n-k)! or in his case (n-1)*n/2
				if(numOfMoving > 0)
				{
					setPairs(&moving,arr);
					//starttime=0;
				}
			} // end of single thread execution
			#pragma omp parallel for
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
						#pragma omp critical // this is critical since the insert of cells is critical and can cause locking
						space.betweenTwoPoints(a,b,&curr); // calculate new path
						ViewPath temp(a,b);
						if(curr.size() > 0)
						{
							temp.cells.assign(curr.begin(),curr.end());
						}
						#pragma omp critical
						paths.insert(pair<pair<PointXY,PointXY>,ViewPath>(key,temp)); // add to memory
						ViewPtr = &paths.at(key); // this wastes some time but makes sure that the path is in memory
					}
					else
					{
						ViewPtr = &(index->second); // get the cells from the past calculated path
					}
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
		} // end of day look

		MPI_Ssend(&one,1,MPI_INT,0,TAG_SENDUPDATE,MPI_COMM_WORLD); // using Ssend to make the slave wait until it starts sending becouse some how when not waiting is puts all messages in the buffer togather if they are small enough.

		MPI_Ssend(&numOfPlanes,1,MPI_INT,0,TAG_UPDATE,MPI_COMM_WORLD);// amount of planes to update
		for(int j=0;j<numOfPlanes;j++)  
		{
			int *msg = planesArr[j]->updatePlaneMessage();
			int bufferSize = 3+msg[2]; // size of the update message (a bit arcane but couldn't think of a better way)
			MPI_Ssend(&bufferSize,1,MPI_INT,0,TAG_UPDATESIZE,MPI_COMM_WORLD);
			MPI_Send(msg,bufferSize,MPI_INT,0,TAG_UPDATE,MPI_COMM_WORLD); // send the msg in the buffer
			free(msg);
			planesArr[j]->resetPlane(); // reset plane to be ready for next time share
		}
		MPI_Send(&one,1,MPI_INT,0,TAG_REQWORK,MPI_COMM_WORLD);// request time piece 
		MPI_Recv(times,2,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);// slave start time and slave end time
	}//end of slave while more work

	//cout << id << ": done" << endl;
	MPI_Finalize();
}

void ParallelProject::run()
{
	fstream stream;
	string str = "C:\\data\\data3.txt";
	map<int,Plane> planes;
	//stream.open(argv[1],ios::in);
	stream.open(str,ios::in);
	space = ProjectSpace(stream);
	int eplanes =0;
	while(stream.good()) // read all planes into memory
	{	 
		Plane temp(stream);
		planes.insert(pair<int,Plane>(temp.flightNumber,temp));
		eplanes++;
	}
	// here was MPI code
	Plane** planesArr = (Plane**)malloc(sizeof(Plane*) * eplanes);
	map<int,Plane>::iterator it = planes.begin();
	for(int i=0;i<eplanes;i++)
	{
		planesArr[i] = &it->second;
		it++;
		//planes.back().print(); //test print
	}
	endtime = DAYINSECONDS;
	starttime = 0;
	map<pair<PointXY,PointXY> ,ViewPath> paths;
	//cout << id <<": starting times " << times[0] << endl;
	for(int i=starttime;i<endtime;i++)
	{
		vector<Plane*> moving; 

#pragma omp parallel for
		for(int j=0; j< eplanes;j++)
		{
			int tid = omp_get_thread_num();
			if(planesArr[j]->isMoving(i))
			{
				int ret = planesArr[j]->step(i,&space);

				if(ret == 0)
				{
					Cell* ptr = planesArr[j]->currentCell;
					if(ptr == NULL)
					{
#pragma omp critical
						{
							ptr = space.insertCell(planesArr[j]->location);
							ptr->occupy(planesArr[j]->flightNumber);
							planesArr[j]->currentCell = ptr;
						}
					}
					else if(!ptr->inCell(planesArr[j]->location)) // the plane is in the same cell he was before
					{
#pragma omp critical
						{
							ptr->leave(planesArr[j]->flightNumber);
							ptr = space.insertCell(planesArr[j]->location);
							ptr->occupy(planesArr[j]->flightNumber);
							planesArr[j]->currentCell = ptr;
						}
					}
#pragma omp critical
					moving.push_back(planesArr[j]);
				}
			}
		}
		int numOfMoving;
		int pairs_number;
		pair<Plane*,Plane*> *arr; // the number of pairs should be n!/k!(n-k)! or in his case (n-1)*n/2
#pragma omp single 
		{
			numOfMoving	 = moving.size();
			pairs_number= (numOfMoving*(numOfMoving-1))/2;
			if(numOfMoving > 0)
			{
				arr = new pair<Plane*,Plane*>[pairs_number];
				setPairs(&moving,arr);
				//starttime=0;
			}
		}	// end of single thread execution
		#pragma omp parallel for	
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
#pragma omp critical
					paths.insert(pair<pair<PointXY,PointXY>,ViewPath>(key,temp)); // add to memory
					ViewPtr = &paths.at(key); // this wastes some time but makes sure that the path is in memory
				}
				else
				{
					ViewPtr = &(index->second); // get the cells from the past calculated path
				}
				
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
	} // end of day look
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
	for(map<int,bool>::iterator it = MaxCL->CDObjects.begin();it!= MaxCL->CDObjects.end();it++)
	{
		cout << it->first << " ";
	}
	cout << endl;
}