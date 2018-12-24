#include "stdafx.h"	
#include "mpi.h"
#include <random>
#include <ctime>
#define SIZE 13

/*int* getRandomMatrix()
{
	int *temp = (int *)malloc(sizeof(int) * (SIZE * SIZE));;

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			temp[i*SIZE + j] = rand() % 100;

	return temp;
}

void showArray2dim(int *a)
{
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			printf("%d  ", a[i*SIZE + j]);
		}
		printf("\n");
	}
}

void showArray1dim(int *a)
{
	for (int i = 0; i < SIZE; i++)
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
	int rows_per_proc = SIZE / (ProcNum - 1);
	bool diff_slices;
	if (SIZE % rows_per_proc != 0)
		diff_slices = true;
	else
		diff_slices = false;

	srand(time(NULL));

	if (ProcRank == 0)
	{
		int *A = getRandomMatrix();
		int *x = new int[SIZE];

		for (int i = 1; i < ProcNum; i++)
		{
			if (!diff_slices)
			{
				MPI_Send(A + (i - 1) * rows_per_proc * SIZE, rows_per_proc * SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
			else
			{
				if (i != ProcNum - 1)
				{
					MPI_Send(A + (i - 1) * rows_per_proc * SIZE, rows_per_proc * SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
				}
				else
				{
					MPI_Send(A + (i - 1) * rows_per_proc * SIZE, (rows_per_proc + SIZE % rows_per_proc) * SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
				}
			}
		}

		for (int i = 1; i < ProcNum; i++)
		{
			int bufElems;
			MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &bufElems);
			int *tempX = new int[bufElems];
			MPI_Recv(tempX, bufElems, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

			for (int j = 0; j < bufElems; j++)
			{
				x[(i - 1)*rows_per_proc + j] = tempX[j];
			}
		}

		printf("Matrix:\n");
		showArray2dim(A);

		printf("Main diagonal:\n");
		showArray1dim(x);
	}
	else
	{
		int bufElems;
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &bufElems);
		int *recvArrA = new int[bufElems];
		MPI_Recv(recvArrA, bufElems, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		int *tempArrX = new int[bufElems / SIZE];
		for (int i = 0; i < bufElems / SIZE; i++)
		{
			tempArrX[i] = recvArrA[SIZE*i + i + (ProcRank - 1)*rows_per_proc];
		}
		MPI_Send(tempArrX, bufElems / SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}*/