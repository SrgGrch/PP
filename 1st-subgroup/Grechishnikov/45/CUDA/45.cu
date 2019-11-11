#include <cuda_runtime.h>
#include "device_launch_parameters.h"

#include <cuda.h>

#include <memory>
#include <conio.h>
#include <iostream>
#include <cassert>
#include <stdio.h>
#include <algorithm>


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

int main(void)
{
	int N = 30109;
	int batchSize = 1024;
	int* x, * d_x, * y, * d_y;
	x = (int*)malloc(batchSize * sizeof(int));
	y = (int*)malloc(batchSize * sizeof(int));

	cudaMalloc(&d_x, batchSize * sizeof(int));
	cudaMalloc(&d_y, batchSize * sizeof(int));

	bool isPrimeFound = false;

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

		dim3 threadsPerBlock(32, 32);
		dim3 numBlocks(batchSize / threadsPerBlock.x, batchSize / threadsPerBlock.y);

		findPrime <<<numBlocks, threadsPerBlock>>> (N, d_x, d_y);
		
		cudaMemcpy(y, d_y, batchSize * sizeof(int), cudaMemcpyDeviceToHost);

		for (int i = 0; i < batchSize; i++) {
			if (y[i] == 1) {
				isPrimeFound = true;
				printf("%d ", x[i]);
				break;
			}
		}
		N = x[batchSize - 1];
	}

	cudaFree(d_x);
	cudaFree(d_y);
	free(x);
	free(y);
	//_getch();
}