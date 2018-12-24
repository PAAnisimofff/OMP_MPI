#include "stdafx.h"
#include "mpi.h"
#include <random>
#include <ctime>

/*int main(int argc, char* argv[]) {
	int x[8];
	int y[8];
	int ProcRank, ProcNum, N = 8;
	int result;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int elements_per_proc = N / ProcNum;
	int *subarr1 = new int[elements_per_proc];
	int *subarr2 = new int[elements_per_proc];
	srand(time(0));
	if (ProcRank == 0) {
		for (int i = 0; i < N; i++)
		{
			x[i] = rand() % 10;
			printf("%d ", x[i]);
		}
		printf("\n");
		for (int i = 0; i < N; i++)
		{
			y[i] = rand() % 10;
			printf("%d ", y[i]);
		}
		printf("\n");
	}
	MPI_Scatter(x, elements_per_proc, MPI_INT,
		subarr1, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(y, elements_per_proc, MPI_INT,
		subarr2, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	int sum = 0;
	for (int i = 0; i < elements_per_proc; i++)
		sum += subarr1[i] * subarr2[i];

	MPI_Reduce(&sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (ProcRank == 0)
		printf("\nscalar mult of x and y = %d\n", result);
	MPI_Finalize();
}*/