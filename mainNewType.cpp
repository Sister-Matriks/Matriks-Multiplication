#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include "mpi.h"
#include "matriks.hpp"

using namespace std;

int main(int argc, char** argv)
{
	int i, j, rank, size, mainProcess, tag, rowSent;
	int sizeMatriks = atoi(argv[1]);
	MPI_Status status;
	
	/* Inisialisasi MPI */
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	/* Inisialisasi dest and src */
	mainProcess = 0;
	tag = 0;
	
	int matriksKanan [sizeMatriks][sizeMatriks];
	int matriksKiri [sizeMatriks][sizeMatriks];
	int **matriksKananTemp;
	int **matriksKiriTemp;
	
	if(size <= sizeMatriks){
		rowSent = sizeMatriks / size;
	}else{
		rowSent = 1;
	}
		
	if(rank == 0) //Inisialisasi matriks kanan
	{
		/* inisialisasi */
		printf("\nno. of process = %d\n", size);
		printf("matrix size = %d\n", sizeMatriks);

		Matriks matriks;
	
		if(argv[1] == NULL)
		{
			printf("Argumen program harus ada, minimal 1. Baca kembali readme program\n");
			exit(1);
		}		
		else
		{
			matriks.setSize(atoi(argv[1]));
			if(argv[2] != NULL)
			{
				matriks.writeMatriks(atoi(argv[2]));
			}
			else
				matriks.writeMatriks();
		}
		
		matriksKananTemp = matriks.read(sizeMatriks);
		matriksKiriTemp = matriks.read(sizeMatriks, true);
		
		for(int i = 0; i < sizeMatriks; i++)
		{
			for(int j = 0; j < sizeMatriks; j++)
			{
				matriksKanan[i][j] = matriksKananTemp[i][j];
				matriksKiri[i][j] = matriksKiriTemp[i][j];
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
		clock_t begin, end;
		double timeSpent;
		
		begin = clock();
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
		
		end = clock();
		timeSpent = (double)(1000 * (end - begin)) / CLOCKS_PER_SEC;
			
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
		
		printf("\nProcessing time: %f ms\n\n", timeSpent);
		
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
