#include "stdafx.h"
#include "mpi.h"
#include <random>
#include <ctime>

/*#define SIZE 8

int* getRandomArr()
{
	int *temp = new int[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		temp[i] = rand() % 10;
	}
	return temp;
}

int* getRandomMatrix()
{
	int *temp = (int *)malloc(sizeof(int) * (SIZE * SIZE));;

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			temp[i*SIZE + j] = rand() % 100;

	return temp;
}

void showArray2dim(int *a)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			printf("%d  ", a[i*SIZE + j]);
		printf("\n");
	}
}

void showArray1dim(int *a)
{
	for (int i = 0; i < SIZE; i++)
		printf("%d ", a[i]);
	printf("\n");
}

int main(int argc, char* argv[]) {
	int *x = new int[SIZE*SIZE];
	int *y = new int[SIZE];
	int *result = new int[SIZE];
	int ProcRank, ProcNum;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int rows_per_proc = SIZE / ProcNum;
	int *submatr = new int[rows_per_proc*SIZE];
	int *subarr = new int[rows_per_proc];
	for (int i = 0; i < rows_per_proc; i++)
		subarr[i] = 0;
	srand(time(0));
	if (ProcRank == 0)
	{
		x = getRandomMatrix();
		y = getRandomArr();
		showArray2dim(x);
		printf("\n");
		showArray1dim(y);
		printf("\n");
	}
	MPI_Scatter(x, rows_per_proc*SIZE, MPI_INT, submatr, rows_per_proc*SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(y, SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < rows_per_proc * SIZE; i++)
		subarr[i/SIZE] += submatr[i] * y[i % SIZE];

	MPI_Gather(subarr, rows_per_proc, MPI_INT, result, rows_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
	if (ProcRank == 0)
		showArray1dim(result);
	MPI_Finalize();
	return 0;
}*/