#include <omp.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>

// Function that converts numbers form LongInt type to double type
double LiToDouble(LARGE_INTEGER x)
{
	double result =
		((double)x.HighPart) * 4.294967296E9 + (double)((x).LowPart);
	return result;
}
// Function that gets the timestamp in seconds
double GetTime()
{
	LARGE_INTEGER lpFrequency, lpPerfomanceCount;
	QueryPerformanceFrequency(&lpFrequency);
	QueryPerformanceCounter(&lpPerfomanceCount);
	return LiToDouble(lpPerfomanceCount) / LiToDouble(lpFrequency);
}

void task_A1()
{
#pragma omp parallel num_threads(8)
	printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
}

void task_A2()
{
	omp_set_dynamic(0);
	omp_set_num_threads(3);
#pragma omp parallel if(omp_get_max_threads()>1)
	{
		printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
	}
	omp_set_num_threads(1);
#pragma omp parallel if(omp_get_max_threads()>1)
	{
		printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
	}
}

void task_A3()
{
	int a = 0, b = 0;

	printf("#before a:%d b:%d\n", a, b);

#pragma omp parallel num_threads(2) private(a) firstprivate(b)
	{
		a = 0;
		a += omp_get_thread_num();
		b += omp_get_thread_num();
		printf("#during%d a:%d b:%d\n", omp_get_thread_num(), a, b);
	}
	printf("#after a:%d b:%d\n\n\n", a, b);

#pragma omp parallel num_threads(4) shared(a) private(b)
	{
		b = 0;
#pragma omp atomic
		a -= omp_get_thread_num();
		b -= omp_get_thread_num();
		printf("#during%d a:%d b:%d\n", omp_get_thread_num(), a, b);
	}
	printf("#after a:%d b:%d\n\n\n", a, b);
}

void task_A4()
{
	int a[10] = { 1, 2, 3, 4, 5, 6, 5, 4, 3, 2 };
	int b[10] = { 10, 2, 3, 4, 5, 1, 5, 4, 3, 2 };

	int min = a[0];
	int max = b[0];

#pragma omp parallel sections num_threads(2)
	{
#pragma omp section
		{
			for (int i = 0; i < 10; i++)
				if (min < a[i])
					min = a[i];
		}

#pragma omp section
		{
			for (int i = 0; i < 10; i++)
				if (max > b[i])
					max = b[i]; 
		}
	}
	printf("max is %d, min is %d\n", min, max);
}

void task_A5()
{
	int d[6][8];
	srand(time(0));
	int N = 30;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			d[i][j] = rand() % N;
			printf("%d ", d[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");

#pragma omp parallel sections
	{
#pragma omp section
		{
			int elements = 0;
			int sum = 0;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					sum += d[i][j];
					elements++;
				}
			}
			printf("mid is %.2f from %d thread\n", sum * 1.0 / elements, omp_get_thread_num());
		}

#pragma omp section
		{
			int min = N + 1;
			int max = -1;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (max > d[i][j])
						max = d[i][j];
					if (min < d[i][j])
						min = d[i][j];
				}
			}
			printf("max is %d and min is %d from %d thread\n", max, min, omp_get_thread_num());
		}

#pragma omp section
		{
			int sum = 0;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (d[i][j] % 3 == 0)
						sum += d[i][j];
				}
			}
			printf("sum of deviders is %d from %d thread\n", sum, omp_get_thread_num());
		}
	}
}


void task_A6()
{
	int a[10] = { 1, 2, 3, 4, 5, 6, 5, 4, 3, 2 };
	int b[10] = { 10, 2, 3, 4, 5, 1, 5, 4, 3, 2 };

	int sum_a = 0, sum_b = 0;
#pragma omp parallel for reduction(+: sum_a) reduction(+: sum_b)
	for (int i = 0; i < 10; i++)
	{
		sum_a += a[i];
		sum_b += b[i];
	}

	printf("%d %d\n", sum_a, sum_b);
}


void task_A7()
{
	const int N = 12;
	int a[N], b[N], c[N];

#pragma omp parallel num_threads(3)
	{
#pragma omp for schedule(static)
		for (int i = 0; i < N; i++)
		{
			a[i] = i * 2;
			b[i] = i * 3;
			printf("working %d of %d threads\n", omp_get_thread_num(), omp_get_num_threads());
		}
	}

	for (int i = 0; i < N; i++)
		printf("%d ", a[i]);
	printf("\n");

	for (int i = 0; i < N; i++)
		printf("%d ", b[i]);
	printf("\n");


#pragma omp parallel num_threads(4)
	{
#pragma parallel omp for schedule(static)
		for (int i = 0; i < N; i++)
		{
			c[i] = a[i] + b[i];
			printf("working %d of %d threads\n", omp_get_thread_num(), omp_get_num_threads());
		}
	}
	for (int i = 0; i < N; i++)
		printf("%d ", c[i]);
	printf("\n");
	

}

void task_A8()
{
	const int N = 16000;
	int a[N];
	double b[N];
	for (int i = 0; i < N; i++)
		a[i] = i;

#pragma omp parallel for schedule(static) num_threads(8) shared(b)
	for (int i = 1; i < N - 1; i++)
	{
		b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
		printf("%.0f:%d ", b[i], omp_get_thread_num());
		if (i == N - 2)
			printf("\n");
	}

#pragma omp parallel for schedule(dynamic) num_threads(8) shared(b)
	for (int i = 1; i < N - 1; i++)
	{
		b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
		printf("%.0f:%d ", b[i], omp_get_thread_num());
		if (i == N - 2)
			printf("\n");
	}

#pragma omp parallel for schedule(dynamic, 3) num_threads(8) shared(b)
	for (int i = 1; i < N - 1; i++)
	{
		b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
		printf("%.0f:%d ", b[i], omp_get_thread_num());
		if (i == N - 2)
			printf("\n");
	}
}

void task_A9()
{
	srand(time(0));
	//std::vector<std::vector<int>> M;
	//std::vector<int> v, tmp;
	//int n = 10, i, j;
	////M.resize(n);
	////v.resize(n);
	//std::cout << "vector:\n";
	//for (i = 0; i < n; i++)
	//{
	//	v.push_back(rand() % 100);
	//	std::cout << v[i] << ' ';
	//}
	//std::cout << std::endl;
	//for (i = 0; i < n; i++)
	//{
	//	for (j = 0; j < n; j++)
	//		tmp.push_back(rand() % 100);
	//	M.push_back(tmp);
	//	tmp.clear();
	//}
	//std::cout << "matrix:\n";
	//for (i = 0; i < n; i++)
	//{
	//	for (j = 0; j < n; j++)
	//		std::cout << M[i][j] << ' ';
	//	std::cout << std::endl;
	//}
	const int n = 10000;
	int i, j;
	int **M = new int *[n];
	for (i = 0; i < n; i++)
		M[i] = new int[n];
	int *v = new int[n];
	double start, finish, time;
	//std::cout << "vector:\n";
	for (i = 0; i < n; i++)
	{
		v[i] = rand() % 100;
		//std::cout << v[i] << ' ';
		for (j = 0; j < n; j++)
			M[i][j] = rand() % 100;
	}
	//std::cout << "\nmatrix:\n";
	/*for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j
		++)
			std::cout << M[i][j] << ' ';
		std::cout << std::endl;
	}*/
	int *c = new int[n];
	//int *M_p = new int[n*n];
	start = GetTime();
	for (i = 0; i < n; i++)
	{
		c[i] = 0;
		for (j = 0; j < n; j++)
			c[i] += M[i][j] * v[j];
	}
	finish = GetTime();
	time = finish - start;
	std::cout <<"Time:" << time << "c.\n";
	start = GetTime();
#pragma omp parallel for schedule(static) num_threads(4) private(j)
	for (i = 0; i < n; i++)
	{
		c[i] = 0;
		for (j = 0; j < n; j++)
			c[i] += M[i][j] * v[j];
	}
	finish = GetTime();
	time = finish - start;
	std::cout << "Time:" << time << "c.\n";
	/*for (i = 0; i < n; i++)
		std::cout << c[i] << ' ';*/
	
	
}

void task_A10()
{
	const int width = 6;
	const int height = 8;
	int N = 30;

	int d[width][height];
	srand(time(0));

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			d[i][j] = rand() % N;
			printf("%d ", d[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");

	int max = d[0][0];
	int min = max;

	int counter = 0;
#pragma omp parallel num_threads(6) reduction(+: counter)
	for (int i = 0; i < width; i++)
	{
#pragma omp for
		for (int j = 0; j < height; j++)
		{
			counter++;
			if (max > d[i][j])
			{
#pragma omp critical
				max = d[i][j];
			}
			if (min < d[i][j])
			{
#pragma omp critical
				min = d[i][j];
			}
		}
	}
	printf("counter is %d\n", counter);
	printf("max is %d, min is %d\n", min, max);
}

void task_A11()
{
	srand(time(0));
	const int n = 30;
	int a[n];
	int d = 9;

	int basic_counter = 0;
	for (int i = 0; i < n; i++)
	{
		a[i] = rand() % n;
		if (a[i] % d == 0)
			basic_counter++;
	}

	int counter = 0;
#pragma omp parallel for shared(counter)
	for (int i = 0; i < n; i++)
	{
		if (a[i] % d == 0)
		{
#pragma omp atomic
			counter++;
		}
	}

	printf("counter is %d\n", counter);
	if (basic_counter == counter)
		printf("the answer is correct\n");
}

void task_A12()
{
	srand(time(0));

	const int n = 30;
	int d = 7;
	int a[n];

	int base_max = -1;
	for (int i = 0; i < n; i++)
	{
		a[i] = rand() % n;
		if (a[i] % d == 0 && base_max < a[i])
			base_max = a[i];
	}

	int max = -1;
#pragma omp parallel for shared(max)
	for (int i = 0; i < n; i++)
	{
		if (a[i] % d == 0 && max < a[i])
		{
#pragma omp critical
			{
				max = a[i];
			}
		}
	}
	printf("%d\n", max);
	if (base_max == max)
		printf("correct\n");
}

int main()
{
	task_A9();
	system("pause");
}