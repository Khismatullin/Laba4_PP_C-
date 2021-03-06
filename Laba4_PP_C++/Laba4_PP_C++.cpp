// Laba4_PP_C++.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <mpi.h>
#include <iostream>
#include <math.h>

using namespace std;

int main(int argc, char **argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Status status;
	MPI_Request request;

	//return count of processors in group(comm) in variable "size"
	MPI_Comm_size(comm, &size);

	//return number of calling processor in group(comm) in variable "rank"
	MPI_Comm_rank(comm, &rank);

	//size of array
	int N = 0;

	//pointer on array
	int *A = nullptr;

	//range of rand number
	int R;

	//magic number
	int M;

	//equal piece for every process
	int piece = 0;

	//storage for count of iterations
	int counterIter = 0;

	//message tag
	int beginTag = 123;

	//message tag
	int stopTag = 321;

	//message tag
	int returnATag = 111;

	//running only on 0 processor
	if (rank == 0)
	{
		//size
		N = 100;

		//range of rand number
		R = 10;

		//magic number
		M = 5;

		//allocate memory for array
		A = new int[N];

		//fill array randomly by range [1, R]
		for (size_t i = 0; i < N; i++)
		{
			A[i] = rand() % R + 1;
		}

		//equal piece for every process
		piece = trunc(N / size);

		//0-process send value of "M" all processes
		MPI_Bcast(&M, sizeof(M), MPI_INT, 0, comm);

		cout << "Array separated between all nodes evenly" << endl;

		

		//output all iterations of processes
		cout << "Process " << "" << "execute " << "" << "iterations" << endl;


	}
	else if (rank != size - 1)
	{
		//all processes wait value "M" from 0-process
		MPI_Recv(&M, sizeof(M), MPI_INT, 0, beginTag, comm, &status);

		//output begin work of process
		cout << "Start work process " << rank << endl;

		//every process calculate your own piece of array
		for (size_t i = rank * piece; i < rank * piece + piece; i++)
		{
			//calculate
			if (A[i] != M)
			{
				A[i] = sin(A[i] - M) * exp(A[i]) - log10(cos(A[i]));

				//update count of iteration for every process
				counterIter++;

				//if already last element of array "A"
				if (i == rank * piece + piece - 1)
				{
					//if not found value "M" in array A
					MPI_Send_init(&A, sizeof(A), MPI_INT, 0, returnATag, comm, &request);
				}
			}
			else
			{
				//delete structures of data associated with this request (request = MPI_Request_null)
				MPI_Request_free(&request);
			}
		}		
	}
	else
	{
		//all processes wait value "M" from 0-process
		MPI_Recv(&M, 1, MPI_INT, 0, beginTag, comm, &status);

		//output begin work of process
		cout << "Start work process " << rank << endl;

		//last process calculate rest
		for (size_t i = rank * piece + piece; i < N; i++)
		{
			//calculate
			if (A[i] != M)
			{
				A[i] = sin(A[i] - M) * exp(A[i]) - log10(cos(A[i]));

				//update count of iteration for every process
				counterIter++;

				//if already last element of array "A"
				if (i == rank * piece + piece - 1)
				{
					//if not found value "M" in array A
					MPI_Send_init(&A, sizeof(A), MPI_INT, 0, returnATag, comm, &request);
				}
			}
			else
			{				
				//delete structures of data associated with this request (request = MPI_Request_null, all operations associated with this request will be stopped)
				MPI_Request_free(&request);
			}
		}

		//if not found value "M" in array A
		MPI_Send_init(&A, sizeof(A), MPI_INT, 0, returnATag, comm, &request);
	}
	
	MPI_Finalize();
	return 0;
}

//terminate all processes in communicator
//MPI_Abort(comm, 404);

//stop and send count of iterations with delay requests
//MPI_Send_init(&counterIter, sizeof(counterIter), MPI_INT, 0, stopTag, comm, &request);

//launch all requests
//MPI_Startall(1, requests);

//delete communicator
//MPI_Comm_free(comm);