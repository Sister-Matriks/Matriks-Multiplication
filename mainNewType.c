#include <stdio.h>
#include <cstdlib>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv)
{
	int i, j, rank, size, dest, src, tag, base, blocklen[2];
	int sizeMatriks = atoi(argv[1]);
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	/* Inisialisasi dest and src */
	src = 0;
	dest = 1;
	tag = 0;
	
	int matriksKanan [sizeMatriks][sizeMatriks];
	int matriksKiri [sizeMatriks / size][sizeMatriks];
		
	if(rank == 0) //Inisialisasi matriks kanan
	{
		/* inisialisasi matrix */
		printf("Inisialisasi matrix\n");
		printf("sizeProcess = %d\n", size);
		printf("sizeMatrix = %d\n", sizeMatriks);
		
		for(int i = 0; i < sizeMatriks; i++)
		{
			for(int j = 0; j < sizeMatriks; j++)
			{
				matriksKanan[i][j] = i;
				if(i < sizeMatriks / size)
				{
					matriksKiri[i][j] = i;
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
					printf("%d", matriksKanan[i][j]);
				else
					printf(" %d", matriksKanan[i][j]);
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
						printf("%d", matriksKiri[i][j]);
					else
						printf(" %d", matriksKiri[i][j]);
			}
			printf("\n");
		}
	}
	
	MPI_Bcast(&matriksKanan[0][0],sizeMatriks*sizeMatriks,MPI_INT,0,MPI_COMM_WORLD);
	
	if(rank == 0) //Jika idProcess = idmaster = 0
	{
		printf("Sending mpi_send from process %d to %d\n", rank, dest);
		MPI_Send(&matriksKiri[0][0], sizeMatriks / size * sizeMatriks, MPI_INT, dest, tag, MPI_COMM_WORLD);
		printf("Message Sent \n");
	}
	else
	{
		int matriksKananRecv [sizeMatriks][sizeMatriks];
		int matriksKiriRecv [sizeMatriks / size][sizeMatriks];
		
		printf("process %d Receiving mpi_recv from %d\n",rank, src);
		MPI_Recv(&matriksKiriRecv[0][0], sizeMatriks / size * sizeMatriks, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
		printf("Message Recv \n");
		
		/* cetak matriks kanan*/
		printf("Matriks kanan after sending\n");
		for(int i = 0; i < sizeMatriks; i++)
		{
			for (int j = 0; j < sizeMatriks; j++)
			{
				if(j == 0)
					printf("%d", matriksKanan[i][j]);
				else
					printf(" %d", matriksKanan[i][j]);
			}
			printf("\n");
		}
		
		/* cetak matriks kiri*/
		printf("\nMatriks kiri after sending\n");
		for(int i = 0; i < sizeMatriks / size; i++)
		{
			for (int j = 0; j < sizeMatriks; j++)
			{
				if(j == 0)
					printf("%d", matriksKiriRecv[i][j]);
				else
					printf(" %d", matriksKiriRecv[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Finalize();
	return 0;
}
