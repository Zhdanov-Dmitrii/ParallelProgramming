#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;


int main()
{
	setlocale(0, "");
	cout << "¬ведите n, m и 0, если надо ввести массив в ручную, иначе любое число\n";
	int n, m, v; cin >> n >> m >> v;
	vector<vector<int>> a(n, vector<int>(m)), b(m, vector<int>(n));
	vector<vector<int>> res(n, vector<int>(n));

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (v)
				a[i][j] = 100 - rand()%200;
			else
				cin >> a[i][j];

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			if (v)
				b[i][j] = 100 - rand() % 200;
			else
				cin >> b[i][j];

	cout << "A\n---------------------------------------------------------------------\n";
	for (auto i : a)
	{
		for (auto j : i)
			cout << j << ' ';
		cout << endl;
	}

	cout << "\n\nB\n---------------------------------------------------------------------\n";
	for (auto i : b)
	{
		for (auto j : i)
			cout << j << ' ';
		cout << endl;
	}

	for (int i = 0; i < n; i++)
#pragma omp parallel for
		for (int j = 0; j < n; j++)		
			for (int k = 0; k < m; k++)
				res[i][j] += a[i][k] * b[k][j];
		
	cout << "\nAxB\n---------------------------------------------------------------------\n";
	for (auto i : res)
	{
		for (auto j : i)
			cout << j << ' ';
		cout << endl;
	}

	return 0;
}
