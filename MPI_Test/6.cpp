
#include "stdafx.h"	
#include <Windows.h>
#include "mpi.h"
#include <chrono>

//#define PING_PONG_LIMIT 10
//#define PING_PING_LIMIT 5
//
//int main(int argc, char** argv) {
//	MPI_Init(NULL, NULL);
//	int ProcRank;
//	int ProcSize;
//	MPI_Status status;
//	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
//	MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
//	clock_t start_time, end_time;
//
//	if (ProcSize != 2) {
//		fprintf(stderr, "World size must be two for %s\n", argv[0]);
//		MPI_Abort(MPI_COMM_WORLD, 1);
//	}
//
//	int ping_pong_count = 0;
//	int ping_ping_count = 0;
//	int partner_rank = (ProcRank + 1) % 2;
//	char message[1000] = "";
//	//ping-pong
//	while (ping_pong_count < PING_PONG_LIMIT) {
//		start_time = clock();
//		if (ProcRank == ping_pong_count % 2) {
//			if (ProcRank == 0)
//				strcat_s(message, "More");
//			MPI_Send(&message, strlen(message), MPI_BYTE, partner_rank, 0, MPI_COMM_WORLD);
//			printf("%d sent %s to %d\n",
//				ProcRank, message, partner_rank);
//		}
//		else {
//			int bufElems;
//			MPI_Probe(partner_rank, 0, MPI_COMM_WORLD, &status);
//			MPI_Get_count(&status, MPI_BYTE, &bufElems);
//			MPI_Recv(&message, bufElems, MPI_BYTE, partner_rank, 0, MPI_COMM_WORLD, &status);
//			end_time = clock();
//			printf("%d received %s from %d for time = %d\n",
//				ProcRank, message, partner_rank, end_time - start_time);
//		}
//		ping_pong_count++;
//	}
//	//ping-ping
//	while (ping_ping_count < PING_PING_LIMIT) {
//		start_time = clock();
//		strcat_s(message, "More");
//		MPI_Send(&message, strlen(message), MPI_BYTE, partner_rank, 0, MPI_COMM_WORLD);
//		printf("%d sent %s to %d\n", ProcRank, message, partner_rank);
//		int bufElems;
//		MPI_Probe(partner_rank, 0, MPI_COMM_WORLD, &status);
//		MPI_Get_count(&status, MPI_BYTE, &bufElems);
//		MPI_Recv(&message, bufElems, MPI_BYTE, partner_rank, 0, MPI_COMM_WORLD, &status);
//		end_time = clock();
//		printf("%d received %s from %d for time = %d\n",
//			ProcRank, message, partner_rank, end_time - start_time);
//		ping_ping_count++;
//	}
//	MPI_Finalize();
//}
