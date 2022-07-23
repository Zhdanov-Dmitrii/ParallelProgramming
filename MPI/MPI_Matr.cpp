#include <iostream>
#include "mpi.h"

using namespace std;


int main(int argc, char** argv)
{
	setlocale(0, "");

	int thread, maxThread;
	MPI_Status status;
	double time;
	double pi, lpi;
	int n, rc, ost;

	int p, v;

	int* a;
	int* b;
	int* c;
	int* lc;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &maxThread);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread);

	if (thread == 0) {
		cout << "n = ";
		cin >> n;
		cout << "";
		cin >> v;
		cout << "";
		cin >> p;
		cout << endl;

		rc = n / maxThread;
		ost = n % maxThread;

		a = new int[n * n];
		b = new int[n * n];
		c = new int[n * n];
		lc = new int[rc * n];

		for (int i = 0; i < n * n; i++)
			if (v)
				a[i] = 100 - rand() % 200;
			else
				cin >> a[i];


		for (int i = 0; i < n * n; i++)
			if (v)
				b[i] = 100 - rand() % 200;
			else
				cin >> b[i];

		if (p)
		{
			cout << "A--------------------------\n";
			for (int x = 0; x < n; ++x)
			{
				for (int y = 0; y < n; ++y)
					cout << a[x * n + y] << " ";
				cout << endl;
			}

			cout << "B--------------------------\n";
			for (int x = 0; x < n; ++x)
			{
				for (int y = 0; y < n; ++y)
					cout << b[x * n + y] << " ";
				cout << endl;
			}
		}

		time = MPI_Wtime();
		for (int i = 1; i < maxThread; i++)
		{
			MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(a, n * n, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(b, n * n, MPI_INT, i, 2, MPI_COMM_WORLD);
		}

		for (int x = 0; x < rc + ost; x++)
			for (int y = 0; y < n; y++)
			{
				c[x * n + y] = 0;
				for (int k = 0; k < n; k++)
					c[x * n + y] += a[x * n + k] * b[k * n + y];
			}

		for (int i = 1; i < maxThread; ++i)
		{
			MPI_Recv(lc, rc * n, MPI_INT, i, 3, MPI_COMM_WORLD, &status);
			for (int x = 0; x < rc * n; x++)
				c[(rc * i + ost) * n + x] = lc[x];
		}
		time = MPI_Wtime() - time;
		if (p)
		{
			cout << "c------------------------------\n";
			for (int x = 0; x < n; ++x)
			{
				for (int y = 0; y < n; ++y)
					cout << c[x * n + y] << " ";
				cout << endl;
			}
		}
		cout << "time - " << time << endl;
	}
	else
	{
		MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		rc = n / maxThread;
		ost = n % maxThread;
		a = new int[n * n];
		b = new int[n * n];
		lc = new int[rc * n];
		MPI_Recv(a, n * n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(b, n * n, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

		for (int x = 0; x < rc; x++)
			for (int y = 0; y < n; y++)
			{
				int t = 0;
				for (int k = 0; k < n; k++)
					t += a[(thread * rc + ost + x) * n + k] * b[k * n + y];
				lc[x * n + y] = t;
			}
		MPI_Send(lc, rc * n, MPI_INT, 0, 3, MPI_COMM_WORLD);

	}



	delete[] a, b, c, lc;

	MPI_Finalize();

	return 0;
}