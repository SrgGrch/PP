#include <cuda_runtime.h>
#include "device_launch_parameters.h"

#include <cuda.h>

#include <memory>
#include <iostream>
#include <cassert>
#include <stdio.h>
#include <algorithm>
#include <ctime>
#include <time.h>


/*
45.     Найти наименьшее простое число, большее заданного N такое, что начало, середина и 
окончание его десятичного представления являются простыми числами (например, число 8353 – 83 5 3)
*/

__global__ void findPrime(int n, int* x, int* y)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

		for (int j = 3; j <= x[i] / 2; j++) {
			if (x[i] % j == 0) return;
		}
	

	int number = x[i];
	int digitsCounter = 0;

	while (number != 0) {
		digitsCounter++;
		number /= 10;
	}

	if (digitsCounter <= 2) return;

	//check end
	if ((x[i] % 10) <= 2) return;

	for (int j = 2; j <= (x[i] % 10)/2; j++) {
		if ((x[i] % 10) % j == 0) return;
	}

	int powRes = 10;

	for (int j = 0; j < digitsCounter-2; j++) {
		powRes *= 10;
	}

	// check start
	if ((x[i] / powRes) <= 2) return;

	for (int j = 2; j <= (x[i] / powRes) / 2; j++) {
		if ((x[i] / (powRes)) % j == 0) return;
	}

	powRes = 10;
	
	for (int j = 0; j < digitsCounter - 3; j++) {
		powRes *= 10;
	}
	// Check middle
	int middle = (x[i] / 10) % powRes;

	if (middle <= 2) return;

	for (int j = 2; j <= middle/2; j++) {
		if (middle % j == 0) return;
	}

	y[i] = 1;
}

bool isPrime(int x)
{
	for (int j = 3; j <= x / 2; j++) {
		if (x % j == 0) return false;
	}


	int number = x;
	int digitsCounter = 0;

	while (number != 0) {
		digitsCounter++;
		number /= 10;
	}

	if (digitsCounter <= 2) return false;

	//check end
	if ((x % 10) <= 2) return false;

	for (int j = 2; j <= (x % 10) / 2; j++) {
		if ((x % 10) % j == 0) return false;
	}

	int powRes = 10;

	for (int j = 0; j < digitsCounter - 2; j++) {
		powRes *= 10;
	}

	// check start
	if ((x / powRes) <= 2) return false;

	for (int j = 2; j <= (x / powRes) / 2; j++) {
		if ((x / (powRes)) % j == 0) return false;
	}

	powRes = 10;

	for (int j = 0; j < digitsCounter - 3; j++) {
		powRes *= 10;
	}
	// Check middle
	int middle = (x / 10) % powRes;

	if (middle <= 2) return false;

	for (int j = 2; j <= middle / 2; j++) {
		if (middle % j == 0) return false;
	}

	return true;
}
	
int main(void)
{
	int Nstart;
		
	printf("Введите начальное число: ");
	scanf("%d", &Nstart);
	int N = Nstart;
	int batchSize = 1024;
	int* x, * d_x, * y, * d_y;
	x = (int*)malloc(batchSize * sizeof(int));
	y = (int*)malloc(batchSize * sizeof(int));

	cudaMalloc(&d_x, batchSize * sizeof(int));
	cudaMalloc(&d_y, batchSize * sizeof(int));

	bool isPrimeFound = false;

	long double start = clock();

	while (!isPrimeFound) {
		if (N % 2 == 0) N++;
		int num = 0;
		for (int i = 0; i < batchSize; i++) {
			x[i] = N + num;
			y[i] = 0;
			num += 2;
		}
	
		std::cout << "Searching in range: " << x[0] << " - " << x[batchSize - 1] << std::endl;

		cudaMemcpy(d_x, x, batchSize * sizeof(int), cudaMemcpyHostToDevice);
		cudaMemcpy(d_y, y, batchSize * sizeof(int), cudaMemcpyHostToDevice);

		dim3 threadsPerBlock(1024, 1, 1);
		dim3 numBlocks(batchSize / threadsPerBlock.x, 1, 1);

		findPrime <<<numBlocks, threadsPerBlock>>> (N, d_x, d_y);
		
		cudaMemcpy(y, d_y, batchSize * sizeof(int), cudaMemcpyDeviceToHost);

		for (int i = 0; i < batchSize; i++) {
			if (y[i] == 1) {
				isPrimeFound = true;
				printf(" Найденное число: %d \n", x[i]);
				printf("GPU: %lf ms\n", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);
				break;
			}
		}
		N = x[batchSize - 1];
	}

	start = clock();

	while (true) {
		if (Nstart % 2 == 0) N++;

		if (isPrime(Nstart)) {
			printf(" Найденное число: %d \n", Nstart);
			printf("CPU: %lf ms\n", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);
			break;
		}
		Nstart++;
	}


	cudaFree(d_x);
	cudaFree(d_y);
	free(x);
	free(y);
}