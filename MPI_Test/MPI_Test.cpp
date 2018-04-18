// MPI_Test.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "mpi.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <omp.h>
#include <random>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;
int n, m;
int length, _size;
inline int _L(int r)
{
	return r * length + 1;
}
inline int R(int r) 
{
	return r == _size - 1 ? n : (r + 1)*length;
}


int inverse(int, int);
void find_inverses(int, int[]);
void swap(int*, int*, int);

#define MAXN 100
#define MAXM 5




struct edge
{
	int u, v, w;
	int id;

	bool operator < (edge e) const
	{
		return w < e.w;
	}
}e[MAXM], e_local[MAXM];

bool in_tree[MAXM];

// Union - Find

int parent[MAXN + 1], uf_rank[MAXN + 1];
int Find(int x)
{
	if (parent[x] == x) return x;
	parent[x] = Find(parent[x]);
	return parent[x];
}

void Union(int x, int y) {
	x = Find(x);
	y = Find(y);

	if (uf_rank[x] < uf_rank[y]) {
		parent[x] = y;
	}
	else {
		parent[y] = x;

		if (uf_rank[x] == uf_rank[y])
			++uf_rank[x];
	}
}

// 

vector<int> L[MAXN], edge_id[MAXN], path, path_edges;
bool visited[MAXN];

bool dfs(int u, int v) {
	path.push_back(u);

	if (u == v) return true;

	bool ret = false;

	if (!visited[u]) {
		visited[u] = true;

		for (int i = 0; i < L[u].size(); ++i) {
			int to = L[u][i];
			path_edges.push_back(edge_id[u][i]);

			if (dfs(to, v)) {
				ret = true;
				break;
			}
			else {
				path_edges.pop_back();
			}
		}
	}

	if (!ret)
		path.pop_back();

	return ret;
}

// 

int mark[MAXN];

void dfs2(int u, int v, int cur) {
	if (visited[cur]) return;
	visited[cur] = true;

	mark[cur] = u;

	for (int i = 0; i < L[cur].size(); ++i) {
		int to = L[cur][i];

		if (to != u && to != v)
			dfs2(u, v, to);
	}
}


class Space {
public:
	int p;					
	int* mult_inv;			
	int* add_inv;			

	Space(int);
};

class Matrix {
public:
	int **aug_matrix;		
	int size;				

	void init_with_file(string, int);
	void display();
	void gauss(Space);
	void Resolve(Space);

};

void eliminate(int*, int*, int, int, Space);

void generateArray(int* vector, size_t size)
{
	const int RND = 10;
	default_random_engine generator(time(0));
	uniform_int_distribution<int> distribution(1, RND);

	for (int i = 0; i < size; i++)
		vector[i] = distribution(generator);
}

int test_size = 4;
/************************************MAIN**************************************/
//-----------------------------Примм---------------------------
int main(int argc, char *argv[])
{
	int p, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(time(0));
	if (rank == 0)
	{
		n = m = 0;
		while (scanf_s("%d,%d,%d", &e[m].u, &e[m].v, &e[m].w) == 3)
		{
			n = max(n, max(e[m].u, e[m].v));
			e[m].id = m;
			++m;
		}
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int blocklengths[] = { 1,1,1,1 };
	MPI_Aint displacements[] = { offsetof(edge,u), offsetof(edge,v), offsetof(edge,w), offsetof(edge,id) };
	MPI_Datatype types[] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT };
	MPI_Datatype MPI_EDGE;

	MPI_Type_create_struct(4, blocklengths, displacements, types, &MPI_EDGE);
	MPI_Type_commit(&MPI_EDGE);

	MPI_Bcast(e, m, MPI_EDGE, 0, MPI_COMM_WORLD);


	for (int i = 1; i <= n; ++i) {
		parent[i] = i;
		uf_rank[i] = 0;
	}

	int start = min(m, (m + p - 1) / p * rank), end = min(m, (m + p - 1) / p * (rank + 1));
	int m_local = 0;

	for (int i = start; i < end; ++i)
		e_local[i - start] = e[i];

	sort(e_local, e_local + (end - start));

	for (int i = 0; i < end - start; ++i) {
		if (Find(e_local[i].u) != Find(e_local[i].v)) {
			Union(e_local[i].u, e_local[i].v);
			e_local[m_local] = e_local[i];
			m_local++;
		}
	}

	int curP = p;

	while (curP > 1) {
		if (rank < curP) {
			int m1 = 0, m2 = 0;
			MPI_Status status;

			MPI_Send(&m_local, 1, MPI_INT, rank / 2, 0, MPI_COMM_WORLD);
			MPI_Send(e_local, m_local, MPI_EDGE, rank / 2, 1, MPI_COMM_WORLD);

			if (2 * rank < curP) {
				MPI_Recv(&m1, 1, MPI_INT, 2 * rank, 0, MPI_COMM_WORLD, &status);
				MPI_Recv(e_local, m1, MPI_EDGE, 2 * rank, 1, MPI_COMM_WORLD, &status);


				if (2 * rank + 1 < curP) {
					MPI_Recv(&m2, 1, MPI_INT, 2 * rank + 1, 0, MPI_COMM_WORLD, &status);
					MPI_Recv(e_local + m1, m2, MPI_EDGE, 2 * rank + 1, 1, MPI_COMM_WORLD, &status);
				}

				int m_aux = m1 + m2;
				sort(e_local, e_local + m_aux);

				for (int i = 1; i <= n; ++i) {
					parent[i] = i;
					uf_rank[i] = 0;
				}

				m_local = 0;

				for (int i = 0; i < m_aux; ++i) {
					if (Find(e_local[i].u) != Find(e_local[i].v)) {
						Union(e_local[i].u, e_local[i].v);
						e_local[m_local] = e_local[i];
						m_local++;
					}
				}
			}
		}

		curP = (curP + 1) / 2;
	}

	if (rank == 0) {
		int total = 0;

		for (int i = 0; i < m_local; ++i) {
			L[e_local[i].u].push_back(e_local[i].v);
			edge_id[e_local[i].u].push_back(e_local[i].id);

			L[e_local[i].v].push_back(e_local[i].u);
			edge_id[e_local[i].v].push_back(e_local[i].id);

			in_tree[e_local[i].id] = true;

			printf("%d - %d (%d)\n", e_local[i].u, e_local[i].v, e_local[i].w);
			total += e_local[i].w;
		}

		printf("Total weight = %d\n\n", total);
	}

	for (int i = 1; i <= n; ++i) {
		int list_size;

		if (rank == 0)
			list_size = L[i].size();

		MPI_Bcast(&list_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

		L[i].resize(list_size);
		MPI_Bcast(&L[i].front(), list_size, MPI_INT, 0, MPI_COMM_WORLD);

		edge_id[i].resize(list_size);
		MPI_Bcast(&edge_id[i].front(), list_size, MPI_INT, 0, MPI_COMM_WORLD);
	}

	MPI_Bcast(in_tree, m, MPI_C_BOOL, 0, MPI_COMM_WORLD);


	bool verified_local = true;

	for (int i = (m + p - 1) / p * rank; i < min(m, (m + p - 1) / p * (rank + 1)); ++i) {
		if (!in_tree[i]) {
			memset(visited, false, sizeof visited);
			path.clear();
			dfs(e[i].u, e[i].v);

			printf("process %d, (%d, %d) : ", rank, e[i].u, e[i].v);

			for (int j = 0; j < path.size(); ++j) {
				if (j > 0) printf(" -> ");
				printf("%d", path[j]);
			}

			printf("\n");

			

			for (int j = 0; j < path_edges.size(); ++j) {
				if (e[i].w < e[path_edges[j]].w)
					verified_local = false;
			}
		}
	}

	bool verified_global;
	MPI_Reduce(&verified_local, &verified_global, 1, MPI_C_BOOL, MPI_LAND, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("\nVerification: ");
		if (verified_global) printf("It is an MST\n\n");
		else printf("It is not an MST\n\n");
	}

	

	int *most_vital_edge_local = new int[p];
	int *max_increase_local = new int[p];
	most_vital_edge_local[rank] = -1;
	max_increase_local[rank] = -1;

	for (int i = (m + p - 1) / p * rank; i < min(m, (m + p - 1) / p * (rank + 1)); ++i) {
		if (in_tree[i]) {
			memset(visited, false, sizeof visited);
			dfs2(e[i].u, e[i].v, e[i].u);
			dfs2(e[i].v, e[i].u, e[i].v);

			printf("process %d, (%d, %d) :", rank, e[i].u, e[i].v);

			int rep = -1;

			for (int j = 0; j < m; ++j)
				if (j != i && ((mark[e[j].u] == e[i].u && mark[e[j].v] == e[i].v) || (mark[e[j].v] == e[i].u && mark[e[j].u] == e[i].v))) {
					printf(" (%d, %d)", e[j].u, e[j].v);

					if (rep == -1 || e[j].w < e[rep].w)
						rep = j;
				}

			if (rep != -1) {
				printf(", replacement : (%d, %d)\n", e[rep].u, e[rep].v);

				if (e[rep].w - e[i].w > max_increase_local[rank]) {
					max_increase_local[rank] = e[rep].w - e[i].w;
					most_vital_edge_local[rank] = i;
				}
			}
			else {
				printf(" no replacement\n");
			}
		}
	}

	if (rank != 0) {
		MPI_Send(&max_increase_local[rank], 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&most_vital_edge_local[rank], 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
	}
	else {
		int most_vital_edge = most_vital_edge_local[0], max_increase = max_increase_local[0];
		MPI_Status status;

		for (int i = 1; i < p; ++i) {
			MPI_Recv(&max_increase_local[i], 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&most_vital_edge_local[i], 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status);

			if (max_increase_local[i] > max_increase) {
				max_increase = max_increase_local[i];
				most_vital_edge = most_vital_edge_local[i];
			}
		}

		printf("\n");
		printf("Most vital edge = (%d, %d)\n", e[most_vital_edge].u, e[most_vital_edge].v);
		printf("Increase = %d\n", max_increase);
	}

	MPI_Finalize();
	system("pause");
	return 0;
}


//-------------------------Кэннон--------------------------------------
//int main(int argc, char *argv[])
//{
//	MPI_Init(&argc, &argv);
//
//	int comm_size, myrank;
//	int my2drank, mycoords[2];
//	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
//	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
//
//
//	MPI_Request requests[4];
//	MPI_Status status[4];
//
//	MPI_Comm comm_2d;
//	int dims[2], periods[2];
//	dims[0] = dims[1] = sqrt(comm_size);
//	periods[0] = periods[1] = 1;
//	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_2d);
//
//	int sizelocal = test_size / dims[0];
//	int *a = new int[sizelocal*sizelocal], *b = new int[sizelocal*sizelocal];
//	generateArray(a, sizelocal*sizelocal);
//	generateArray(b, sizelocal*sizelocal);
//
//	MPI_Comm_rank(comm_2d, &my2drank);
//	MPI_Cart_coords(comm_2d, my2drank, 2, mycoords);
//
//	int uprank, downrank, leftrank, rightrank;
//	MPI_Cart_shift(comm_2d, 0, -1, &rightrank, &leftrank);
//	MPI_Cart_shift(comm_2d, 1, -1, &downrank, &uprank);
//
//	int *buffersA[2], *buffersB[2];
//
//	buffersA[0] = a;
//	buffersA[1] = (int *)malloc(sizelocal*sizelocal * sizeof(int));
//	buffersB[0] = b;
//	buffersB[1] = (int *)malloc(sizelocal*sizelocal * sizeof(int));
//
//	int shiftsource, shiftdest;
//	MPI_Cart_shift(comm_2d, 0, -mycoords[0], &shiftsource, &shiftdest);
//	MPI_Sendrecv_replace(buffersA[0], sizelocal*sizelocal, MPI_INT, shiftdest, 1, shiftsource, 1, comm_2d, &status[0]);
//
//	MPI_Cart_shift(comm_2d, 1, -mycoords[1], &shiftsource, &shiftdest);
//	MPI_Sendrecv_replace(buffersB[0], sizelocal*sizelocal, MPI_INT, shiftdest, 1, shiftsource, 1, comm_2d, &status[0]);
//
//	printf("Rank %d at point 1\n", my2drank);
//
//	for (int i = 0; i < 2; i++)
//	{
//		printf("Rank %d started iteration %d\n", my2drank, i);
//		MPI_Isend(buffersA[i % 2], sizelocal*sizelocal, MPI_INT, leftrank, 1, comm_2d, &requests[0]);
//		MPI_Isend(buffersB[i % 2], sizelocal*sizelocal, MPI_INT, uprank, 1, comm_2d, &requests[1]);
//		MPI_Irecv(buffersA[(i + 1) % 2], sizelocal*sizelocal, MPI_INT, rightrank, 1, comm_2d, &requests[2]);
//		MPI_Irecv(buffersB[(i + 1) % 2], sizelocal*sizelocal, MPI_INT, downrank, 1, comm_2d, &requests[3]);
//		MPI_Waitall(4, requests, status);
//		printf("Rank %d stopped waiting in iteration %d\n", my2drank, i);
//		MPI_Barrier(comm_2d);
//	}
//
//	printf("Rank %d at point 2\n", my2drank);
//
//	MPI_Barrier(MPI_COMM_WORLD);
//	MPI_Finalize();
//	return 0;
//}

//-------------------Гаусс--------------------------
//int main(int argc, char *argv[]) {
//	int *pivot_row, *row;
//	string filename;
//
//	int numprocs, myid;
//
//	MPI_Init(&argc, &argv);
//	numprocs = MPI_COMM_WORLD;
//	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
//
//
//	int p = 7;				
//	Space modp(p);
//
//	Matrix system;
//	system.init_with_file("coefficients.txt", p);
//
//	if (myid == 0) {
//		cout << endl << system.size << endl << endl;
//		system.display();
//	}
//
//	
//	system.Resolve(modp);    
//
//	if (myid == 0) {
//		system.display();
//	}
//
//	MPI_Finalize();
//
//	return 0;
//}
//----------------------------Чет-нечет сортировка---------------
//int main(int argc, char* argv[])
//{
//	int rnk, size;
//	int n, length;
//	int *buffer;
//
//	double tIO, tTMP, tSTART, tEND, tCOMM;
//	n = atoi(argv[1]);
//	buffer = new int[n + 1];
//
//	MPI_Status status;
//	MPI_File in, out;
//
//	MPI_Init(&argc, &argv);
//
//	tSTART = MPI_Wtime();
//
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
//
//	MPI_File_open(MPI_COMM_WORLD, argv[2], MPI_MODE_RDONLY, MPI_INFO_NULL, &in);
//	MPI_File_open(MPI_COMM_WORLD, argv[3], MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &out);
//
//	
//	if (n <= size || size == 1) {
//		int left = 1;
//		int right = (rnk == 0 ? n : 0);
//
//		tTMP = MPI_Wtime();                                              /*  store datas from buffer[1] */
//		MPI_File_read_at_all(in, (left - 1) * sizeof(int), buffer + left, right - left + 1, MPI_INT, &status);
//		MPI_File_close(&in);
//		tIO += MPI_Wtime() - tTMP; //Input
//
//		if (rnk == 0) sort(buffer + 1, buffer + 1 + n);
//		MPI_Barrier(MPI_COMM_WORLD);
//
//		tTMP = MPI_Wtime();
//		MPI_File_write_at_all(out, (left - 1) * sizeof(int), buffer + left, right - left + 1, MPI_INT, &status);
//		MPI_File_close(&out);
//		tIO += MPI_Wtime() - tTMP; //Output
//
//		if (rnk == 0) {
//			tEND = MPI_Wtime();
//			printf("Total: %f\n", tEND - tSTART);
//			printf("IO: %f\n", tIO);
//			printf("COMM: %f\n", tCOMM);
//			printf("CPU: %f\n", tEND - tSTART - tIO - tCOMM);
//		}
//
//		MPI_Finalize();
//		return 0;
//	}
//
//	length = n / size;
//	int left = _L(rnk), right = R(rnk);
//
//	tTMP = MPI_Wtime();
//	MPI_File_read_at_all(in, (left - 1) * sizeof(int), buffer + left, right - left + 1, MPI_INT, &status);
//	MPI_File_close(&in);
//	tIO += MPI_Wtime() - tTMP; 
//
//	sort(buffer + _L(rnk), buffer + R(rnk) + 1); 
//	int OddToEven = 1;
//	while (1) {
//		if (OddToEven == 1) { 
//			if (rnk % 2 == 0 && rnk + 1<size) {
//				tTMP = MPI_Wtime();
//				MPI_Recv(&buffer[_L(rnk + 1)], R(rnk + 1) - _L(rnk + 1) + 1, MPI_INT, rnk + 1, rnk, MPI_COMM_WORLD, &status);
//				tCOMM += MPI_Wtime() - tTMP;
//			}
//			else if (rnk % 2 == 1 && rnk<size) {
//				tTMP = MPI_Wtime();
//				MPI_Send(&buffer[_L(rnk)], R(rnk) - _L(rnk) + 1, MPI_INT, rnk - 1, rnk - 1, MPI_COMM_WORLD);
//				tCOMM += MPI_Wtime() - tTMP;
//			}
//
//			if (rnk % 2 == 0 && rnk + 1<size) {
//				sort(buffer + _L(rnk), buffer + R(rnk + 1) + 1);
//				
//				tTMP = MPI_Wtime();
//				MPI_Send(&buffer[_L(rnk + 1)], R(rnk + 1) - _L(rnk + 1) + 1, MPI_INT, rnk + 1, rnk, MPI_COMM_WORLD);
//				tCOMM += MPI_Wtime() - tTMP;
//
//			}
//			else if (rnk % 2 == 1 && rnk<size) {
//			
//				tTMP = MPI_Wtime();
//				MPI_Recv(&buffer[_L(rnk)], R(rnk) - _L(rnk) + 1, MPI_INT, rnk - 1, rnk - 1, MPI_COMM_WORLD, &status);
//				tCOMM += MPI_Wtime() - tTMP;
//			}
//		}
//		else { // 2-->1-->2  4-->3-->4
//			if (rnk % 2 == 0 && rnk - 1>0)
//			{
//				tTMP = MPI_Wtime();
//				MPI_Send(&buffer[_L(rnk)], R(rnk) - _L(rnk) + 1, MPI_INT, rnk - 1, rnk - 1, MPI_COMM_WORLD);
//				tCOMM += MPI_Wtime() - tTMP;
//			}
//			else if (rnk % 2 == 1 && rnk + 1<size) 
//			{
//				tTMP = MPI_Wtime();
//				MPI_Recv(&buffer[_L(rnk + 1)], R(rnk + 1) - _L(rnk) + 1, MPI_INT, rnk + 1, rnk, MPI_COMM_WORLD, &status);
//				tCOMM += MPI_Wtime() - tTMP;
//			}
//
//			if (rnk % 2 == 0 && rnk - 1>0)
//			{
//				tTMP = MPI_Wtime();
//				MPI_Recv(&buffer[_L(rnk)], R(rnk) - _L(rnk) + 1, MPI_INT, rnk - 1, rnk - 1, MPI_COMM_WORLD, &status);
//				tCOMM += MPI_Wtime() - tTMP;
//			}
//			else if (rnk % 2 == 1 && rnk + 1<size) 
//			{
//				
//				sort(buffer + _L(rnk), buffer + R(rnk + 1) + 1);
//				tTMP = MPI_Wtime();
//				MPI_Send(&buffer[_L(rnk + 1)], R(rnk + 1) - _L(rnk + 1) + 1, MPI_INT, rnk + 1, rnk, MPI_COMM_WORLD);
//				tCOMM += MPI_Wtime() - tTMP;
//			}
//		}
//
//		
//		if (rnk == size - 1) {
//			tTMP = MPI_Wtime();
//			MPI_Send(&buffer[_L(rnk)], n - _L(rnk) + 1, MPI_INT, rnk - 1, rnk, MPI_COMM_WORLD);
//			tCOMM += MPI_Wtime() - tTMP;
//		}
//		else if (rnk == 0) {
//			tTMP = MPI_Wtime();
//			MPI_Recv(&buffer[_L(rnk + 1)], n - _L(rnk + 1) + 1, MPI_INT, rnk + 1, rnk + 1, MPI_COMM_WORLD, &status);
//			tCOMM += MPI_Wtime() - tTMP;
//		}
//		else 
//		{
//			
//			tTMP = MPI_Wtime();
//			MPI_Recv(&buffer[_L(rnk + 1)], n - _L(rnk + 1) + 1, MPI_INT, rnk + 1, rnk + 1, MPI_COMM_WORLD, &status);
//			
//			MPI_Send(&buffer[_L(rnk)], n - _L(rnk) + 1, MPI_INT, rnk - 1, rnk, MPI_COMM_WORLD);
//			tCOMM += MPI_Wtime() - tTMP;
//		}
//
//
//		int is_sorted = 1;
//		if (rnk == 0) {
//			is_sorted = 1;
//			for (int i = 1; i <= n - 1; i++) {
//				if (buffer[i]>buffer[i + 1]) {
//					is_sorted = 0;
//					break;
//				}
//			}
//			tTMP = MPI_Wtime();
//			MPI_Send(&is_sorted, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); //send to next
//			tCOMM += MPI_Wtime() - tTMP;
//		}
//		else if (rnk == size - 1) {
//			tTMP = MPI_Wtime();
//			MPI_Recv(&is_sorted, 1, MPI_INT, rnk - 1, rnk - 1, MPI_COMM_WORLD, &status);
//			tCOMM += MPI_Wtime() - tTMP;
//		}
//		else {
//			tTMP = MPI_Wtime();
//			MPI_Recv(&is_sorted, 1, MPI_INT, rnk - 1, rnk - 1, MPI_COMM_WORLD, &status);
//			MPI_Send(&is_sorted, 1, MPI_INT, rnk + 1, rnk, MPI_COMM_WORLD);
//			tCOMM += MPI_Wtime() - tTMP;
//		}
//
//		if (is_sorted == 1) break;
//		OddToEven = (OddToEven + 1) % 2;
//	}
//
//	tTMP = MPI_Wtime();
//	MPI_File_write_at_all(out, (left - 1) * sizeof(int), buffer + left, right - left + 1, MPI_INT, &status);
//	MPI_File_close(&out);
//	tIO += MPI_Wtime() - tTMP; //Output
//
//	MPI_Barrier(MPI_COMM_WORLD);
//
//	if (rnk == 0) {
//		tEND = MPI_Wtime();
//		printf("Total: %f\n", tEND - tSTART);
//		printf("IO: %f\n", tIO);
//		printf("COMM: %f\n", tCOMM);
//		printf("CPU: %f\n", tEND - tSTART - tIO - tCOMM);
//	}
//
//	MPI_Finalize();
//
//	return 0;
//}

/****************************FUNCTION DEFINITIONS******************************/

int inverse(int p, int num) {
	if (num == 1)
		return 1;

	if (p%num == 0) {
		cerr << "\nError: \n\tProvided value is not a prime number.\n" << endl;
		exit(1);
	}

	else {
		int n = num - inverse(num, p%num);
		return (n*p + 1) / num;
	}
}

void find_inverses(int p, int inv_arr[]) {
#pragma omp parallel for
	for (int i = 1; i < p; i++)
		inv_arr[i] = inverse(p, i);
	return;
}

void swap(int* row_a, int* row_b, int size) {
	int temp;
#pragma omp parallel for
	for (int i = 0; i < size; i++) {
		temp = row_a[i];
		row_a[i] = row_b[i];
		row_b[i] = temp;
	}
}


void eliminate(int* pivotrow, int *row, int pivot, int size, Space modp) {
	int n = modp.add_inv[row[pivot] % modp.p];
#pragma omp parallel for
	for (int i = 0; i < size + 1; i++) {
		row[i] = row[i] + (pivotrow[i] * n);
		row[i] = row[i] % modp.p;
	}
}

Space::Space(int n) {
	p = n;
	int inv;

	mult_inv = new int[p];
	add_inv = new int[p];

	
	find_inverses(n, mult_inv);
	
	add_inv[0] = 0;
	
	for (int i = 1; i < p / 2 + 1; i++) {
		inv = p - i;
		add_inv[i] = inv;
		add_inv[inv] = i;
	}
}

void Matrix::display() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size + 1; j++)
			cout << aug_matrix[i][j] << "\t";
		cout << endl;
	}
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	return;
}

void Matrix::init_with_file(string filename, int prime) {

	ifstream infile;
	int temp_val, rows = 0, take;
	string temp_line;
	bool tokenerror;

	infile.open(filename.c_str(), fstream::in);
	if (infile.fail()) {
		cerr << "\nError: \n\tFile could not be opened\n" << endl;
		exit(1);
	}

	while (getline(infile, temp_line))
	{
		rows++;
	}

	infile.close();

	infile.open(filename.c_str(), fstream::in);
	if (infile.fail()) {
		cerr << "\nError: \n\tFile could not be opened\n" << endl;
		exit(1);
	}


	aug_matrix = new int*[rows];
	for (int i = 0; i < rows; i++) {
		aug_matrix[i] = new int[rows + 1];
		for (int j = 0; j < rows + 1; j++) {
			infile >> temp_val;
			aug_matrix[i][j] = temp_val % prime;
		}
	}

	size = rows;
	infile.close();
	return;
}



void Matrix::Resolve(Space modp)
{

	int numprocs, myid, tag = 0;

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	int *temp_array = new int[size + 1], *temp_array_pivot = new int[size + 1];
	int i, j, k, inv, j_row;

	int size_chunck, size_chuck_tail;
	//int temp_chunck*[size+1];

	if (myid != numprocs - 1) {
		//chunksize = size/numprocs; 				
	}
	else if (myid == numprocs - 1) {
		//chunksize = range - (myid*size/numprocs)+2; 				
	}

	
	for (i = 0; i < size; i++)
	{
		if (myid == 0) {
			if (aug_matrix[i][i] == 0)
			{
				
				for (j = i + 1; j < size; j++)
				{
					if (aug_matrix[j][i] != 0)
					{
						swap(aug_matrix[i], aug_matrix[j], size);
						break;
					}
				}
				if (j >= size) {
					cout << endl << "System does not have a unique solution" << endl;
					exit(1);
				}
			}
		}

	}

	
	for (i = 0; i < size; i++) {
		if (myid == 0) {
	
			if (aug_matrix[i][i] != 1) {
				inv = modp.mult_inv[aug_matrix[i][i]];
				for (j = 0; j < size + 1; j++) {     //parallel for
					aug_matrix[i][j] = (aug_matrix[i][j] * inv) % modp.p;
				}
			}

			temp_array_pivot = aug_matrix[i];
			k = 1;
		}

		MPI_Bcast(temp_array_pivot, size + 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&i, 1, MPI_INT, 0, MPI_COMM_WORLD);

		for (j = i + 1; j < size; j++) {

			MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

			if (myid == 0) {
				MPI_Send(aug_matrix[j], size + 1, MPI_INT, k, tag, MPI_COMM_WORLD);
				MPI_Send(&j, 1, MPI_INT, k, tag, MPI_COMM_WORLD); 
			}

			if (myid == k) { // k is going to depend on how many n there are.
				MPI_Recv(temp_array, size + 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(&j_row, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				eliminate(temp_array_pivot, temp_array, i, size, modp);

				MPI_Send(&j_row, 1, MPI_INT, 0, tag, MPI_COMM_WORLD); 
				MPI_Send(temp_array, size + 1, MPI_INT, 0, tag, MPI_COMM_WORLD);


			}
			if (myid == 0) {
				k++;
				if (k == numprocs) {
					k = 1;
				}
			}


		}

		

		if (myid == 0) {
			k = 1;
		}
		for (j = i + 1; j < size; j++) {
			
			if (myid == 0) {
				MPI_Recv(&j_row, 1, MPI_INT, k, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(aug_matrix[j_row], size + 1, MPI_INT, k, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
			}

			if (myid == 0) {
				k++;
				if (k == numprocs) {
					k = 1;
				}

			}
		}

		MPI_Barrier(MPI_COMM_WORLD);

	}

	MPI_Barrier(MPI_COMM_WORLD);


	for (i = size - 1; i >= 1; i--) {

		if (myid == 0) {
			temp_array_pivot = aug_matrix[i];
			k = 1;
		}

		
		MPI_Bcast(temp_array_pivot, size + 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&i, 1, MPI_INT, 0, MPI_COMM_WORLD);

		for (j = i - 1; j >= 0; j--) {

			MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

			if (myid == 0) {
				MPI_Send(aug_matrix[j], size + 1, MPI_INT, k, tag, MPI_COMM_WORLD);
				MPI_Send(&j, 1, MPI_INT, k, tag, MPI_COMM_WORLD); 
			}

			if (myid == k)
			{ 
				MPI_Recv(temp_array, size + 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(&j_row, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				eliminate(temp_array_pivot, temp_array, i, size, modp); 

				MPI_Send(&j_row, 1, MPI_INT, 0, tag, MPI_COMM_WORLD); 
				MPI_Send(temp_array, size + 1, MPI_INT, 0, tag, MPI_COMM_WORLD);


			}
			if (myid == 0)
			{
				k++;
				if (k == numprocs)
					k = 1;
			}

		}

		if (myid == 0)
			k = 1;

		for (j = i - 1; j >= 0; j--)
		{
			if (myid == 0) {
				MPI_Recv(&j_row, 1, MPI_INT, k, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(aug_matrix[j_row], size + 1, MPI_INT, k, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
			}

			if (myid == 0) {
				k++;
				if (k == numprocs) {
					k = 1;
				}

			}
		}

		MPI_Barrier(MPI_COMM_WORLD);

	}

	MPI_Barrier(MPI_COMM_WORLD);

	return;
}


void Matrix::gauss(Space modp) {

	int numprocs, myid, tag = 0;

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);


	int i, j, k, inv;
	int *temp_array = new int[size + 1], *temp_array_pivot = new int[size + 1];
	

	for (i = 0; i < size; i++) {
		if (myid == 0) {
			if (aug_matrix[i][i] == 0) {
				
				for (j = i + 1; j < size; j++) {
					if (aug_matrix[j][i] != 0) {
						swap(aug_matrix[i], aug_matrix[j], size);
						break;
					}
				}
				if (j >= size) {
					cout << endl << "System does not have a unique solution" << endl;
					exit(1);
				}
			}
			if (aug_matrix[i][i] != 1) {
				
				inv = modp.mult_inv[aug_matrix[i][i]];
				for (j = 0; j < size + 1; j++) {     //parallel for
					aug_matrix[i][j] = (aug_matrix[i][j] * inv) % modp.p;
				}
			}

			temp_array_pivot = aug_matrix[i];
		}
		MPI_Bcast(temp_array_pivot, size + 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&i, 1, MPI_INT, 0, MPI_COMM_WORLD);

		if (myid == 0) {
			k = 1;
		}
		for (j = i + 1; j < size; j++) {

			MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

			
			if (myid == 0) { //
				MPI_Send(aug_matrix[j], size + 1, MPI_INT, k, tag, MPI_COMM_WORLD);
			}

		
			if (myid == k) {
				MPI_Recv(temp_array, size + 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				eliminate(temp_array_pivot, temp_array, i, size, modp); 

				MPI_Send(temp_array, size + 1, MPI_INT, 0, tag, MPI_COMM_WORLD);


			}

			if (myid == 0) {
				k++;
				if (k == numprocs) {
					k = 1;
				}
			}

		}
		//MPI_Barrier();

		if (myid == 0) {
			k = 1;
		}
		for (j = i + 1; j < size; j++) {
			
			if (myid == 0) {
				MPI_Recv(aug_matrix[j], size + 1, MPI_INT, k, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				for (int o = 0; o < size + 1; o++) {
					cout << aug_matrix[j][o] << " ";
				}
				cout << endl;
			}

			if (myid == 0) {
				k++;
				if (k == numprocs) {
					k = 1;
				}
				display();
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);




	for (i = size - 1; i >= 1; i--)
	{

		
		temp_array_pivot = aug_matrix[i];
		MPI_Bcast(temp_array_pivot, size + 1, MPI_INT, 0, MPI_COMM_WORLD);

		k = 1;
		for (j = i - 1; j >= 0; j--) {
			/*
			cont = false;
			if(myid == 0){
			if( aug_matrix[j][j] != 0) {
			cont = true;
			}
			}
			*/
			//MPI_Bcast(&cont, 1, MPI::BOOL, 0);
			//      if(cont){

		
			if (myid == 0) {
				MPI_Send(aug_matrix[j], size + 1, MPI_INT, k, tag, MPI_COMM_WORLD);
			}
			//id == j%numprocs Recieve;
			if (myid == k) {
				MPI_Recv(temp_array, size + 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				eliminate(temp_array_pivot, temp_array, i, size + 1, modp); 

				MPI_Send(temp_array, size + 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
			}
			k++;
			if (k == numprocs) {
				k = 1;
			}
			//      }
		}
		//MPI_Barrier();

		k = 1;
		for (j = i - 1; j >= 0; j--) {
			/*
			cont = false;
			if(myid == 0){
			if( aug_matrix[j][j] != 0) {
			cont = true;
			}
			}
			*/
		

			
			if (myid == 0) {
				MPI_Recv(aug_matrix[j], size + 1, MPI_INT, k, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				for (int o = 0; o < size + 1; o++) {
					cout << aug_matrix[j][o] << " ";
				}
				cout << endl;
			}
			k++;
			if (k == numprocs) {
				k = 1;
			}
			//       }
			if (myid == 0) {
				display();
				cout << endl << endl << endl;
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	return;
}

//int main(int argc, char *argv[])
//{
//	MPI_Init(&argc, &argv);
//	int rank;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	if (rank == 0)
//	{
//		char helloStr[] = "Hello world";
//		MPI_Send(helloStr, _countof(helloStr), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
//	}
//	else
//		if (rank == 1)
//		{
//			char helloStr[12];
//			MPI_Recv(helloStr, _countof(helloStr), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//			printf("Rank 1 received string %s from Rank 0\n", helloStr);
//		}
//	MPI_Finalize();
//    return 0;
//}

