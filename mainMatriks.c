#include <mpi.h>
#include <stdio.h>
#include "matriks.hpp"

int main(int argc,char *argv[]) {
	int numtasks, rank;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	
	Matriks matriks(10);
	
	if(argv[1] == NULL)
		matriks.writeMatriks();
	else
		matriks.writeMatriks(atoi(argv[1]));
	
	return 0;
}
