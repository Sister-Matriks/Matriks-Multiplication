#include <stdio.h>
#include <cstdlib>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv)
{
	int i, j, rank, size, mainProcess, tag, rowSent;
	int sizeMatriks = atoi(argv[1]);
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	/* Inisialisasi dest and src */
	mainProcess = 0;
	tag = 0;
	
	int matriksKanan [sizeMatriks][sizeMatriks];
	int matriksKiri [sizeMatriks][sizeMatriks];
	
	if(size <= sizeMatriks){
		rowSent = sizeMatriks / size;
	}else{
		rowSent = 1;
	}
		
	if(rank == 0) //Inisialisasi matriks kanan
	{
		/* inisialisasi */
		printf("\nno. of processes = %d\n", size);
		printf("matrix size = %d\n", sizeMatriks);
		
		for(int i = 0; i < sizeMatriks; i++)
		{
			for(int j = 0; j < sizeMatriks; j++)
			{
				matriksKanan[i][j] = i;
				matriksKiri[i][j] = i * size;
			}
		}
		
		/* cetak matriks kanan*/
		printf("\nMatriks kanan:\n");
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
		printf("\nMatriks kiri:\n");
		for(int i = 0; i < sizeMatriks; i++)
		{
			for (int j = 0; j < sizeMatriks; j++)
			{
					if(j == 0)
						printf("%d", matriksKiri[i][j]);
					else
						printf(" %d", matriksKiri[i][j]);
			}
			printf("\n");
		}
	}
	
	MPI_Bcast(&matriksKanan[0][0], sizeMatriks*sizeMatriks, MPI_INT, mainProcess, MPI_COMM_WORLD);
	
	if(rank == 0) //Jika idProcess = idmaster = 0
	{
		int matriksResult [sizeMatriks][sizeMatriks];
		
		int offset = 0;
		for(int dest = 1; dest < size; dest++)
		{
			if(dest < sizeMatriks){
				offset = dest * rowSent;
				//printf("offset before sending ke process %d = %d\n", dest, offset);
				//printf("Sending mpi_send from process %d to %d\n", rank, dest);
				MPI_Send(&matriksKiri[offset][0], rowSent * sizeMatriks, MPI_INT, dest, tag, MPI_COMM_WORLD);
				//printf("Message Sent \n");
				MPI_Recv(&matriksResult[offset][0], rowSent * sizeMatriks, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
			}
		}
		
		// matrix multiplication calculation for process-0
		for(int i = 0; i < rowSent; i++){
			for(int j = 0; j < sizeMatriks; j++){
				int sum = 0;
				for(int k = 0; k < sizeMatriks; k++){
					sum = sum + matriksKiri[i][k] * matriksKanan[k][j]; 
				}
				matriksResult[i][j] = sum;
			}
		}
			
		printf("\nMatriks hasil perkalian (kiri X kanan):\n");
		for(int i = 0; i < sizeMatriks; i++)
		{
			for (int j = 0; j < sizeMatriks; j++)
			{
				if(j == 0)
					printf("%d", matriksResult[i][j]);
				else
					printf(" %d", matriksResult[i][j]);
			}
			printf("\n");
		}
		
	}
	else
	{
		if(rank < sizeMatriks){
			int matriksKiriRecv [rowSent][sizeMatriks];
			int offsetRecv = rank * rowSent;
			
			int matriksResult [rowSent][sizeMatriks];
			
			//printf("process %d Receiving mpi_recv from %d\n",rank, mainProcess);
			//printf("offset = %d\n", offsetRecv);
			MPI_Recv(&matriksKiriRecv[0][0], rowSent * sizeMatriks, MPI_INT, mainProcess, tag, MPI_COMM_WORLD, &status);
			//printf("Message Recv \n");
			
			// matrix multiplication calculation
			for(int i = 0; i < rowSent; i++){
				for(int j = 0; j < sizeMatriks; j++){
					int sum = 0;
					for(int k = 0; k < sizeMatriks; k++){
						sum = sum + matriksKiriRecv[i][k] * matriksKanan[k][j]; 
					}
					matriksResult[i][j] = sum;
				}
			}	
			
			MPI_Send(&matriksResult[0][0], rowSent * sizeMatriks, MPI_INT, mainProcess, tag, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}
