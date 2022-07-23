#include <iostream>
#include <vector>
#include<omp.h>

using namespace std;

void merge(vector<int>& vec, int l, int mid, int r)
{
	int j = mid;
	int i = l;
	int k = 0;

	vector<int> res(r - l);
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

}

int main()
{
	setlocale(0, "");
	cout << "n = ";
	int n; cin >> n;
	int v, p;
	cout << "0, если надо ввести данные в ручную ";
	cin >> v;
	cout << "0, чтобы не выводить результат";

	vector<int> vec(n);
	for (int i = 0; i < n; i++)
	{
		if (v)
		{
			vec[i] = rand();
			cout << vec[i] << ' ';
		}
		else
			cin >> vec[i];
	}
	cout << endl;

	double time = omp_get_wtime();
	for (int i = 1; i < n; i *= 2)
#pragma omp parallel for
		for (int j = 0; j < n - i; j += 2 * i)
			merge(vec, j, j + i, min(j + 2 * i, n));

	time = omp_get_wtime() - time;

	if(p)
		for (int i : vec)
			cout << i << ' ';

	cout << endl << "time - " << time << endl;


	return 0;
}

