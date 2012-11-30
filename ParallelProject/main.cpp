#include "ParallelProject.h"
#define _DIST 0

void main(int argc, char* argv[]) { 
	//omp_set_num_threads(NUM_THREADS);		
	ParallelProject program;
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

