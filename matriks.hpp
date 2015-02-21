#ifndef MATRIKS_H
#define MATRIKS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

class Matriks
{
	private:
		int size;
		ofstream myfile;
		string matriks;
		int randNumber;
		
	public:
		Matriks(int _N)
		{
			size	= _N;
			
		}
		void setMatriksSize(int _N)
		{
			size	= _N;
		}
		
		int getMatriksSize()
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
};
#endif
