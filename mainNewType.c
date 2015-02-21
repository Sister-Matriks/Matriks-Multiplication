#include <stdio.h>
#include <cstdlib>
#include "mpi.h"

typedef struct _cell {
	int **MatriksKanan; /* cell coordinates */
	int **MatriksKiri;
}Process;

Process process, processReceive;
using namespace std;

int main(int argc, char** argv)
{
	int i, j, rank, size, dest, src, tag, base, blocklen[2];
	int sizeMatriks = 4;//atoi(argv[1]);
	MPI_Datatype NewType;
	MPI_Datatype type[] = {MPI_INT, MPI_INT};
	MPI_Aint disp[2], intex;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* compute displacements of structure components */
	MPI_Type_extent(MPI_INT, &intex);
	disp[0] = 0;
	disp[1] = intex* 4 * 4;//sizeMatriks*sizeMatriks;
	
	/* compute blocklength */
	blocklen[0] = sizeMatriks * sizeMatriks;
	blocklen[1] = (sizeMatriks / size ) * sizeMatriks;
	
	MPI_Type_struct(2, blocklen, disp, type, &NewType);

	MPI_Type_commit(&NewType);
	
	/* Inisialisasi dest and src */
	src = 0;
	dest = 1;
	tag = 0;
	
	if(!rank) //Jika idProcess = idmaster = 0
	{
		/* inisialisasi matrix */
		printf("Inisialisasi matrix\n");
		printf("sizeProcess = %d\n", size);
		printf("sizeMatrix = %d\n", sizeMatriks);
		process.MatriksKanan = new int*[sizeMatriks];
		process.MatriksKiri = new int*[sizeMatriks];
		for(int i = 0; i < sizeMatriks; i++)
		{
			process.MatriksKanan[i] = new int[sizeMatriks];
			if(i < sizeMatriks / size)
			{
				process.MatriksKiri[i] = new int[sizeMatriks];
			}
		}
		for(int i = 0; i < sizeMatriks; i++)
		{
			for(int j = 0; j < sizeMatriks; j++)
			{
				process.MatriksKanan[i][j] = i;
				if(i < sizeMatriks / size)
				{
					process.MatriksKiri[i][j] = i;
				}
			}
		}
		
		/* cetak matriks kanan*/
		printf("Matriks kanan\n");
		for(int i = 0; i < sizeMatriks; i++)
		{
			for (int j = 0; j < sizeMatriks; j++)
			{
				if(j == 0)
					printf("%d", process.MatriksKanan[i][j]);
				else
					printf(" %d", process.MatriksKanan[i][j]);
			}
			printf("\n");
		}
		
		/* cetak matriks kiri*/
		printf("\nMatriks kiri\n");
		for(int i = 0; i < sizeMatriks; i++)
		{
			for (int j = 0; j < sizeMatriks; j++)
			{
				if(i < sizeMatriks / size)
					if(j == 0)
						printf("%d", process.MatriksKiri[i][j]);
					else
						printf(" %d", process.MatriksKiri[i][j]);
			}
			printf("\n");
		}

		printf("Sending mpi_send from process %d to %d\n", rank, dest);
		MPI_Send(&process, sizeMatriks * sizeMatriks, NewType, dest, tag, MPI_COMM_WORLD);
		printf("Message Sent \n");
	}
	else
	{
		printf("process %d Receiving mpi_recv from %d\n",rank, src);
		MPI_Recv(&processReceive, sizeMatriks * sizeMatriks, NewType, src, tag, MPI_COMM_WORLD, &status);
		printf("Message Recv \n");
		
		/* cetak matriks kanan*/
		printf("Matriks kanan after sending\n");
		for(int i = 0; i < sizeMatriks; i++)
		{
			for (int j = 0; j < sizeMatriks; j++)
			{
				if(j == 0)
					printf("%d", processReceive.MatriksKanan[i][j]);
				else
					printf(" %d", processReceive.MatriksKanan[i][j]);
			}
			printf("\n");
		}
		
		/* cetak matriks kiri*/
		printf("\nMatriks kiri after sending\n");
		for(int i = 0; i < sizeMatriks; i++)
		{
			for (int j = 0; j < sizeMatriks; j++)
			{
				if(i < sizeMatriks / size)
					if(j == 0)
						printf("%d", processReceive.MatriksKiri[i][j]);
					else
						printf(" %d", processReceive.MatriksKiri[i][j]);
			}
			printf("\n");
		}
	}
	
	MPI_Type_free(&NewType);
	MPI_Finalize();
}
