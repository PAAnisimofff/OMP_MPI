#include "stdafx.h"
#include "mpi.h"
#include <random>
#include <ctime>

#define SIZE_1 10
#define SIZE_2 8

int main(int argc, char* argv[])
{
	char buff[1000];
	int a[SIZE_1];
	double b[SIZE_2];
	int ProcRank, ProcNum;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int position;
	
	if (ProcRank == 0)
	{
		srand(time(0));
		for (int i = 0; i < SIZE_1; i++)
		{
			a[i] = rand() % 100;
			if (i < SIZE_2)
				b[i] = (double)(rand()) / RAND_MAX * 100;
		}
		printf("Array of 10 int A:\n");
		for (int i = 0; i < SIZE_1; i++)
			printf("%d ",a[i]);
		printf("\nArray of 8 double B:\n");
		for (int i = 0; i < SIZE_2; i++)
			printf("%.2f ", b[i]);

		position=0;
		MPI_Pack(a, SIZE_1, MPI_INT, buff, 1000, &position, MPI_COMM_WORLD);
		MPI_Pack(b, SIZE_2, MPI_DOUBLE, buff, 1000, &position, MPI_COMM_WORLD);
	}
	MPI_Bcast(buff, 1000, MPI_PACKED, 0, MPI_COMM_WORLD);
	
	if (ProcRank != 0)
	{
		position = 0;
		MPI_Unpack(buff, 1000, &position, a, SIZE_1, MPI_INT, MPI_COMM_WORLD);
		MPI_Unpack(buff, 1000, &position, b, SIZE_2, MPI_DOUBLE, MPI_COMM_WORLD);

		printf("Array of 10 int A from process %d:\n", ProcRank);
		for (int i = 0; i < SIZE_1; i++)
			printf("%d ", a[i]);
		printf("\nArray of 8 double B from process %d:\n", ProcRank);
		for (int i = 0; i < SIZE_2; i++)
			printf("%.2f ", b[i]);
	}

	MPI_Finalize();
}