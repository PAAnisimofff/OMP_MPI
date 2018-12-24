#include "stdafx.h"
#include "mpi.h"
#include <random>

/*#define SIZE 8

int main(int argc, char* argv[])
{
	int a[SIZE][SIZE];
	int b[SIZE / 2][SIZE];
	int c[SIZE / 2][SIZE];
	int ProcRank, ProcNum;
	MPI_Status status;
	MPI_Datatype type;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	int count = 4, blocklength = SIZE, stride = SIZE * 2;
	MPI_Type_vector(count, blocklength, stride, MPI_INT, &type);
	MPI_Type_commit(&type);

	if (ProcRank == 0)
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				a[i][j] = rand() % 100;
		printf("Array A:\n");
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
				printf("%d  ", a[i][j]);
			printf("\n");
		}
		MPI_Send(&a[0][0], 1, type, 1, 0, MPI_COMM_WORLD);
		MPI_Send(&a[1][0], 1, type, 1, 1, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(&a[0][0], 1, type, 0, 0, MPI_COMM_WORLD, &status);
		printf("Array C:\n");
		for (int i = 0; i < SIZE; i += 2)
		{
			for (int j = 0; j < SIZE; j++)
			{
				c[i / 2][j] = a[i][j];
				printf("%d  ", c[i / 2][j]);
			}
			printf("\n");
		}
		MPI_Recv(&a[1][0], 1, type, 0, 1, MPI_COMM_WORLD, &status);
		printf("Array B:\n");
		for (int i = 1; i < SIZE; i += 2)
		{
			for (int j = 0; j < SIZE; j++)
			{
				b[i / 2][j] = a[i][j];
				printf("%d  ", b[i / 2][j]);
			}
			printf("\n");
		}
	}

	MPI_Type_free(&type);
	MPI_Finalize();
}*/