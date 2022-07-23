#include <iostream>
#include "mpi.h"

using namespace std;


int main(int argc, char** argv)
{
	int thread, maxThread;
	double start;
	double pi,lpi;
	unsigned long long N;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &maxThread);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread);

	if (thread == 0)
	{
		cin >> N;
		for (int i = 1; i < maxThread; i++)
			MPI_Send(&N, 1, MPI_UNSIGNED_LONG_LONG, i, 0, MPI_COMM_WORLD);
	}

	MPI_Status status;
	if (thread != 0)
		MPI_Recv(&N, 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD, &status);

	lpi = 0;
	for(int i = thread; i < N; i += maxThread)
		lpi += 4. / (1. + (i + 0.5) * (i + 0.5) * 1 / (N * N));
	lpi /= N;

	if (thread != 0)
		MPI_Send(&lpi, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);

	if (thread == 0)
	{
		pi = lpi;
		for (int i = 1; i < maxThread; i++)
		{
			MPI_Recv(&lpi, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
		}
	}



	MPI_Finalize();
	return 0;
}