#include <iostream>
#include <omp.h>

using namespace std;


int main()
{
	long long N; cin >> N;
	double pi = 0;

	double start = omp_get_wtime();

#pragma omp parallel for reduction(+:pi)
	for (long long i = 1; i < N; i++)
		pi += 4. / (1. + (i + 0.5) * (i + 0.5) * 1/(N*N));


	pi /= N;	

	double time = omp_get_wtime() - start;

	cout << pi << endl;
	cout << time;

	return 0;
}