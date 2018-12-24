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
	MPI_Status status;
	int elements_per_proc = SIZE / (ProcNum - 1);
	bool diff_slices;
	if (SIZE % elements_per_proc != 0)
		diff_slices = true;
	else
		diff_slices = false;
	int bufElems;

	srand(time(NULL));

	if (ProcRank == 1)
	{
		int *a = new int[SIZE];
		for (int i = 0; i < SIZE; i++)
			a[i] = rand() % 100;

		MPI_Send(a, elements_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD);

		for (int i = 2; i < ProcNum; i++)
		{
			if(!diff_slices)
				MPI_Send(a+(i-1)*elements_per_proc, elements_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
			else
			{
				if (i != ProcNum - 1)
					MPI_Send(a + (i - 1)*elements_per_proc, elements_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
				else
					MPI_Send(a + (i - 1)*elements_per_proc, elements_per_proc + SIZE % elements_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}
	}
	else
	{
		MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &bufElems);
		int *subarr = new int[bufElems];
		MPI_Recv(subarr, bufElems, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

		printf("From process %d: ", ProcRank);
		for (int i = 0; i < bufElems; i++)
			printf("%d ", subarr[i]);
	}

	MPI_Finalize();
	return 0;
}*/