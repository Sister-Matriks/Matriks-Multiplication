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

#include <stdio.h>
#include "matriks.hpp"
using namespace std;

int main(int argc,char *argv[]) {
	
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
	
	int **matriks1;
	int **matriks2;
	matriks1 = matriks.read(atoi(argv[1]));
	matriks2 = matriks.read(atoi(argv[1]), true);
	
	printf("\nMatriks pertama \n");
	matriks.cetak(matriks1, atoi(argv[1]));
	
	printf("\nMatriks kedua \n");
	matriks.cetak(matriks2, atoi(argv[1]));
		
	
	return 0;
}
