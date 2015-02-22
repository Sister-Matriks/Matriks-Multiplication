#ifndef MATRIKS_H
#define MATRIKS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;

class Matriks
{
	private:
		int size;
		ofstream myfile;
		ifstream in_stream;
		string matriks;
		int randNumber;
		
	public:
		Matriks()
		{
			size = 4;
		}
		
		Matriks(int _N)
		{
			size	= _N;
			
		}
		void setSize(int _N)
		{
			size	= _N;
		}
		
		int getSize()
		{
			return size;
		}
		
		void writeMatriks()
		{
			myfile.open ("matriks.txt");
			srand((unsigned)time(0));
			for(int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					randNumber = rand() % 19 + (-9);
					if(j == 0)
						myfile << randNumber;
					else
						myfile << " " << randNumber;
				}
				myfile << "\n";
			}
			myfile.close();
		}
		
		void writeMatriks(int value)
		{
			myfile.open ("matriks.txt");
			for(int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if(j == 0)
						myfile << value;
					else
						myfile << " " << value;
				}
				myfile << "\n";
			}
			myfile.close();
		}
		
		void cetak (int **matrix, int size)
		{
			int i;
			int j;

			for(i=0; i<size; i++)
			{
				for(j=0; j<size; j++)
				{
					printf("%d ", matrix[i][j]);
				}
				printf("\n");
			}
		}
		
		void cetak (int **matrix)
		{
			int size = 4;
			int i;
			int j;

			for(i=0; i<size; i++)
			{
				for(j=0; j<size; j++)
				{
					printf("%d ", matrix[i][j]);
				}
				printf("\n");
			}
		}
		
		int** read(int size)
		{
			int ** matriks;
			in_stream.open("matriks.txt");
			matriks = new int*[size];
			for(int i = 0; i < size; i++)
			{
				matriks[i] = new int[size];
			}
			string number;
			int i=0, j=0, counter = 0;
			
			for(i = 0; i < size; i++)
			{
				for(j = 0; j < size; j++)
				{
					in_stream >> number;
					matriks[i][j] = atoi(number.c_str());
				}
				if(counter == size*size)
					break;
				else
					counter++;
			}
			in_stream.close();
			return matriks;
		}
		
		int** read()
		{
			int size = 4;
			int ** matriks;
			in_stream.open("matriks.txt");
			matriks = new int*[size];
			for(int i = 0; i < size; i++)
			{
				matriks[i] = new int[size];
			}
			string number;
			int i=0, j=0, counter = 0;
			
			for(i = 0; i < size; i++)
			{
				for(j = 0; j < size; j++)
				{
					in_stream >> number;
					matriks[i][j] = atoi(number.c_str());
				}
				if(counter == size*size)
					break;
				else
					counter++;
			}
			in_stream.close();
			return matriks;
		}
};
#endif
