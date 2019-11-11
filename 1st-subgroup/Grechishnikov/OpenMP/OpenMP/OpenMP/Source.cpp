#include <cmath>
#include <chrono>
#include <iostream>
#include <string>
#include <time.h>
#include <omp.h>


using namespace std::chrono;
using namespace std;

void main() {
	const int size = 10240;
	double sinTable[size];
	double sinTable2[size];

	cout << omp_get_max_threads();

	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	for (int n = 0; n < size; ++n)
		sinTable2[n] = std::sin(2 * 3.14 * n / size);

	cout << std::to_string(start.count() - duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) << endl;

	start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

#pragma omp target teams distribute parallel for map(from:sinTable[0:10240])
	for (int n = 0; n < size; ++n)
		sinTable[n] = std::sin(2 * 3.14 * n / size);
	
	cout << std::to_string(start.count() - duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) << endl;

	// the table is now initialized
}