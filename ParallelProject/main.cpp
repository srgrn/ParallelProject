#include "ParallelProject.h"
#define _DIST 0

void main(int argc, char* argv[]) { 
	omp_set_num_threads(NUM_THREADS);		
	ParallelProject program = ParallelProject();
	program.mpi_init(argc,argv);
#ifdef _DIST 
	if(program.id==0)  // load from file on first computer
	{
		cout << "master" << endl;;
		program.master();
	}
	else // slave asks for work	
	{
		cout << "slave " << program.id << endl;;
		program.slave();
	}
#else
	program.run();
#endif
	
}

