// MpiSolution.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"	
#include "mpi.h"
#include <random>
#include <ctime>
/*#define SIZE 10

int* getRandomArr()
{
	int *temp = new int[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		temp[i] = rand() % 100;
	}
	return temp;
}

int* sum(int *a, int *b, int size)
{
	int *temp = new int[size];
	for (int i = 0; i < size; i++) {
		temp[i] = a[i] + b[i];
	}
	return temp;
}

int* mult(int *a, int *b, int size)
{
	int *temp = new int[size];
	for (int i = 0; i < size; i++) {
		temp[i] = a[i] * b[i];
	}
	return temp;
}

void showArray(int *a, int size)
{
	for (int i = 0; i < size; i++)
		printf("%d ", a[i]);
	printf("\n");
}

int main(int argc, char **argv)
{
	int ProcRank, ProcNum;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Status status;
	int elements_per_proc = SIZE / (ProcNum - 1);
	bool diff_slices;
	if (SIZE % elements_per_proc != 0)
		diff_slices = true;
	else
		diff_slices = false;

	srand(time(NULL));

	if (ProcRank == 0)
	{
		int *x = getRandomArr();
		int *y = getRandomArr();
		int *z = new int[SIZE];

		for (int i = 1; i < ProcNum; i++)
		{
			if (!diff_slices)
			{
				MPI_Send(x + (i - 1)*elements_per_proc, elements_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(y + (i - 1)*elements_per_proc, elements_per_proc, MPI_INT, i, 1, MPI_COMM_WORLD);
			}
			else
			{
				if (i != ProcNum - 1)
				{
					MPI_Send(x + (i - 1)*elements_per_proc, elements_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
					MPI_Send(y + (i - 1)*elements_per_proc, elements_per_proc, MPI_INT, i, 1, MPI_COMM_WORLD);
				}
				else
				{
					MPI_Send(x + (i - 1)*elements_per_proc, elements_per_proc + SIZE % elements_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
					MPI_Send(y + (i - 1)*elements_per_proc, elements_per_proc + SIZE % elements_per_proc, MPI_INT, i, 1, MPI_COMM_WORLD);
				}
			}
		}

		for (int i = 1; i < ProcNum; i++)
		{
			int bufElems;
			MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &bufElems);
			int *tempZ = new int[bufElems];
			MPI_Recv(tempZ, bufElems, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

			for (int j = 0; j < bufElems; j++)
			{
				z[(i-1)*elements_per_proc + j] = tempZ[j];
			}
		}

		printf("Array X:\n");
		showArray(x, SIZE);
		printf("Array Y:\n");
		showArray(y, SIZE);
		printf("Array Z:\n");
		showArray(z, SIZE);
	}
	else
	{
		int bufElems;
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &bufElems);
		int *recvArrX = new int[bufElems];
		MPI_Recv(recvArrX, bufElems, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		int *recvArrY = new int[bufElems];
		MPI_Recv(recvArrY, bufElems, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		int *tempArrZ = mult(recvArrX, recvArrY, bufElems);

		MPI_Send(tempArrZ, bufElems, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}*/