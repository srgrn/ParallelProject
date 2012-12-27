#include "ParallelProject.h"
#define _DIST 0 //commenting out this will allow running without MPI by calling the run function instead of the master and slave

void main(int argc, char* argv[]) { 
	omp_set_num_threads(NUM_THREADS);		
	ParallelProject program = ParallelProject();
	program.mpi_init(argc,argv);
#ifdef _DIST 
	if(program.id==0)  // load from file on first computer
	{
		program.master();
	}
	else // slave asks for work	
	{
		program.slave();
	}
#else
	program.run();
#endif

}

