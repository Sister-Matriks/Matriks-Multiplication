/* how to use
 * Default, size matriks 4x4 random value
 * ./hasil-compile
 * 
 * Jika mau menentukan size matriks, tapi isinya random
 * ./hasil-compile size-matriks
 * 
 * jika matriks hanya punya 1 jenis Value
 * ./hasil-compile size-matriks value
 * 
*/

#include <mpi.h>
#include <stdio.h>
#include "matriks.hpp"
using namespace std;

int main(int argc,char *argv[]) {
	int numtasks, rank;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	
	Matriks matriks;
	
	if(argv[1] != NULL)
	{
		matriks.setSize(atoi(argv[1]));
		if(argv[2] != NULL)
		{
			matriks.writeMatriks(atoi(argv[2]));
		}
		else
			matriks.writeMatriks();
	}		
	else
		matriks.writeMatriks();
	
	int **matriksN;
	if(argv[1] == NULL)
	{
		matriksN = matriks.read();
		matriks.cetak(matriksN);
	}
	else
	{
		matriksN = matriks.read(atoi(argv[1]));
		matriks.cetak(matriksN, atoi(argv[1]));
	}
		
	
	return 0;
}
