#include "stdafx.h"
#include "mpi.h"
#include <random>
#include <ctime>

/*#define SIZE 8

int main(int argc, char* argv[])
{
	int a[SIZE][SIZE];
	int ProcRank, ProcNum;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int blocklength[SIZE], disp[SIZE];
	MPI_Datatype ntype;

	blocklength[0] = SIZE;
	disp[0] = 0;
	for (int i = 1; i < SIZE; i++)
	{
		blocklength[i] = SIZE - i;
		disp[i] = SIZE * i + i;
	}

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			a[i][j] = 0;

	MPI_Type_indexed(SIZE, blocklength, disp, MPI_INT, &ntype);
	MPI_Type_commit(&ntype);

	if (ProcRank == 0)
	{
		srand(time(0));
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				if (j >= i)
					a[i][j] = rand() % 100;
		printf("Array A:\n");
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
				printf("%d ", a[i][j]);
			printf("\n");
		}
	}
	MPI_Bcast(a, 1, ntype, 0, MPI_COMM_WORLD);

	if (ProcRank != 0)
	{
		printf("Array A from process %d:\n", ProcRank);
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
				printf("%d ", a[i][j]);
			printf("\n");
		}
	}

	MPI_Type_free(&ntype);
	MPI_Finalize();
}*/