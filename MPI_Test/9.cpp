#include "stdafx.h"
#include "mpi.h"
#include <random>
#include <ctime>

#define SIZE 8

/*struct val_ind
{
	float value;
	int index;
};

float* getRandomArr()
{
	float *temp = new float[SIZE];
	for (int i = 0; i < SIZE; i++)
		temp[i] = rand() % 100 / 12.f;
	return temp;
}

void showArray(float *a)
{
	for (int i = 0; i < SIZE; i++)
		printf("%f ", a[i]);
	printf("\n");
}

int main(int argc, char* argv[]) {
	float *x = new float[SIZE];
	val_ind result;
	int ProcRank, ProcNum;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	int elements_per_proc = SIZE / ProcNum;
	float *subarr = new float[elements_per_proc];

	if (ProcRank == 0)
	{
		srand(time(0));
		x = getRandomArr();
		showArray(x);
	}

	MPI_Scatter(x, elements_per_proc, MPI_FLOAT, subarr, elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

	val_ind temp;
	temp.value = subarr[0];
	temp.index = ProcRank * elements_per_proc;
	for (int i = 1; i < elements_per_proc; i++)
		if (temp.value > subarr[i])
		{
			temp.value = subarr[i];
			temp.index = ProcRank * elements_per_proc + i;
		}

	MPI_Reduce(&temp, &result, 1, MPI_FLOAT_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

	if (ProcRank == 0)
		printf("min value = %f at index %d\n", result.value, result.index);

	MPI_Finalize();
}*/