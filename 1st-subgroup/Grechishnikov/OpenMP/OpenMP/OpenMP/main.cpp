#include <cmath>
#include <iostream>
#include <ctime>
#include <omp.h>

using namespace std;

int main() {
	const int size = 40000;
	double sinTable[size];
	double sinTable2[size];

	long double start = clock();

	for (int n = 0; n < size; ++n)
		sinTable2[n] = std::sin(2 * 3.14 * n / size);

	cout << (clock() - start) / CLOCKS_PER_SEC * 1000 << endl;

	start = clock();

#pragma omp parallel for private (sinTable) default (none)
	for (int n = 0; n < size; ++n)
		sinTable[n] = std::sin(2 * 3.14 * n / size);

	cout << (clock() - start) / CLOCKS_PER_SEC * 1000 << endl;

	// the table is now initialized
	return 0;
}