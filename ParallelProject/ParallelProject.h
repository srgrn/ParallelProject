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

class ParallelProject
{
public:
	ParallelProject(void);
	~ParallelProject(void);
	void mpi_init(int argc,char* argv[]);
	void setPairs(vector<Plane*>* vec,pair<Plane*,Plane*> *arr); // this bit of code is creating pairs of stuff
	void setPairs(Plane* planesArr,int numOfPlanes,pair<Plane*,Plane*> *arr);
	void master();
	void slave();
	void run();
	
	// MPI Variables
	int  namelen;
	int numprocs;
	int id;
	MPI_Status status;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	// End of MPI Variables
	
	int times[2];
	ProjectSpace space;

};

