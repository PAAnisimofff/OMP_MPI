// MpiSolution.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"	
#include "mpi.h"
#include <random>
#include <ctime>

#define SIZE 10

/*int main(int argc, char **argv)
{
	int ProcRank, ProcNum;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	printf("%d  %d\n", ProcRank, ProcNum);

	srand(time(NULL));

	if (ProcRank == 0)
	{
		int *a = new int[SIZE];
		for (int i = 0; i < SIZE; i++)
		{
			a[i] = rand() % 100;
		}
		MPI_Send(a, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		int *a = new int[SIZE];
		MPI_Recv(a, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Elements of a:\n");
		for (int i = 0; i < SIZE; i++)
		{
			printf("%d ", a[i]);
		}
	}

	MPI_Finalize();
	return 0;
}*/