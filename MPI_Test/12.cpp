#include "stdafx.h"
#include "mpi.h"
#include <random>

/*#define SIZE 8

struct MyType
{
	bool p;
	char c[10];
	int d;
	float f;
};


int main(int argc, char* argv[])
{
	MyType a[SIZE][SIZE];
	MyType b[SIZE / 4][SIZE];
	int ProcRank, ProcNum;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int blocklength[2] = { SIZE, SIZE };

	MPI_Datatype types[2] = { MPI_INT, MPI_INT };
	MPI_Aint disp[2];
	MPI_Datatype newtype, resNewtype;

	disp[0] = 0;
	disp[1] = sizeof(int) * SIZE * 4;

	MPI_Type_create_struct(2, blocklength, disp, types, &newtype);
	MPI_Type_commit(&newtype);
	int lb;
	//MPI_Type_create_resized(&newtype, );

	if (ProcRank == 0)
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				a[i*SIZE + j]->d = rand() % 100;
		printf("Array A:\n");
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
				printf("%d  ", a[i*SIZE + j]);
			printf("\n");
		}
	}

	MPI_Scatter(a, 1, resNewtype, a, 1, resNewtype, 0, MPI_COMM_WORLD);

	printf("Elements of D from %d process\n", ProcRank);
	for (int i = 0; i < SIZE / 4; i++)
	{
		for (int j = 0; j < SIZE; j++)
			printf("%d  ", a[i*SIZE + j]->d);
		printf("\n");
	}

	MPI_Type_free(&newtype);
	MPI_Finalize();
	return 0;
}*/