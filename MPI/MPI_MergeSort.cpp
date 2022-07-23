#include <iostream>

#include "mpi.h"

using namespace std;

void merge(int* vec, int l, int mid, int r)
{
	int j = mid;
	int i = l;
	int k = 0;

	int* res = new int[r - l];
	while (i < mid && j < r)
	{
		if (vec[i] < vec[j])
		{
			res[k] = vec[i];
			i++;
		}
		else
		{
			res[k] = vec[j];
			j++;
		}

		k++;
	}

	while (i < mid)
	{
		res[k] = vec[i];
		i++; k++;
	}

	while (j < r)
	{
		res[k] = vec[j];
		j++; k++;
	}

	for (i = l, k = 0; i < r; i++, k++)
		vec[i] = res[k];

	delete[] res;
}

int main(int argc, char** argv)
{
	int thread, maxThread;
	MPI_Status status;
	double start;

	int* vec;
	int* tvec;
	int n, ln, ln2, ost;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &maxThread);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread);

	if (thread == 0)
	{
		cout << "n = ";
		cin >> n;

		ln = n / maxThread;
		ost = n % maxThread;

		vec = new int[n];
		for (int i = 0; i < n; i++)
		{
			vec[i] = rand() % 100;
			cout << vec[i] << " ";
		}
		cout << endl;
		for (int i = 1; i < maxThread; i++)
		{
			MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(vec + ost + ln * i, ln, MPI_INT, i, 1, MPI_COMM_WORLD);
		}

		ln += ost;

		for (int i = 1; i < ln; i *= 2)
			for (int j = 0; j < ln - i; j += 2 * i)
				merge(vec, j, j + i, min(j + 2 * i, n));
	}
	else
	{
		MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		ln = n / maxThread;
		vec = new int[n];
		MPI_Recv(vec, ln, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		for (int i = 1; i < ln; i *= 2)
			for (int j = 0; j < ln - i; j += 2 * i)
				merge(vec, j, j + i, min(j + 2 * i, n));

	}

	for (int i = 1; i < maxThread; i *= 2)
	{
		for (int j = 0; j < maxThread - i; j += 2 * i)
		{
			if (thread == j)
			{
				MPI_Recv(&ln2, 1, MPI_INT, j + i, j + i, MPI_COMM_WORLD, &status);
				tvec = new int[ln2];

				MPI_Recv(tvec, ln2, MPI_INT, j + i, j + i, MPI_COMM_WORLD, &status);
				for (int i = 0; i < ln2; i++)
					vec[ln + i] = tvec[i];
				merge(vec, 0, ln, ln + ln2);
				ln += ln2;
				delete[] tvec;
			}
		}
		for (int j = i; j < maxThread; j += 2 * i)
		{
			if (thread == j)
			{
				MPI_Send(&ln, 1, MPI_INT, j - i, j, MPI_COMM_WORLD);
				MPI_Send(vec, ln, MPI_INT, j - i, j, MPI_COMM_WORLD);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}


	if (thread == 0)
		for (int i = 0; i < n; i++)
			cout << vec[i] << " ";
	delete[] vec;

	MPI_Finalize();

	return 0;
}