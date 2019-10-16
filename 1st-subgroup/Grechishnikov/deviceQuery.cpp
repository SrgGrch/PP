/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

 /*
  * This is a simple test program to measure the memcopy bandwidth of the GPU.
  * It can measure device to device copy bandwidth, host to device copy bandwidth
  * for pageable and pinned memory, and device to host copy bandwidth for pageable
  * and pinned memory.
  *
  * Usage:
  * ./bandwidthTest [option]...
  */

  // CUDA runtime
#include <cuda_runtime.h>

#include <cuda.h>

#include <memory>
#include <iostream>
#include <cassert>

__global__ void process() {



}

__host__ int main() {



}
