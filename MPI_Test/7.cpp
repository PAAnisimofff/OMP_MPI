#include "stdafx.h"
#include "mpi.h"
#include <random>
#include <ctime>
#define SIZE 10

/*int* getRandomArr()
{
	int *temp = new int[SIZE];
	for (int i = 0; i < SIZE; i++)
		temp[i] = rand() % 10;
	return temp;
}

void showArray(int *a)
{
	for (int i = 0; i < SIZE; i++)
		printf("%d ", a[i]);
	printf("\n");
}

int main(int argc, char **argv)
{
	int *x = new int[SIZE], norm;
	int ProcRank, ProcNum;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	int elements_per_proc = SIZE / ProcNum;
	int *subarr = new int[elements_per_proc];

	int localSum;
	if (ProcRank == 0)
	{
		srand(time(0));
		x = getRandomArr();
		showArray(x);
	}
	MPI_Scatter(x, elements_per_proc, MPI_INT, subarr, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	localSum = 0;
	for (int i = 0; i < elements_per_proc; i++)
		localSum += subarr[i];

	MPI_Reduce(&localSum, &norm, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (ProcRank == 0)
		printf("\nnorm of x = %d\n", norm);
	MPI_Finalize();
	return 0;
}*/