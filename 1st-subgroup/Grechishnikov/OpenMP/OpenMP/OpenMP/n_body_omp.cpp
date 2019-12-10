/* Sequential version of N body simulation */
/* Author: Raghav Pandya */

#include "NBody.h"
#include "VectorMath.h"
#include "CycleTimer.h"
#include <omp.h>
#include <algorithm>

using namespace std;

// Compute forces on each body with time step

void display_bodies()
{
	for (int i = 0; i < BODY_COUNT; i++)
	{
		printf("\nBody %d:\nMass: %f\nPosition(x ,y, z): %f, %f, %f\nVelocity(x, y, z): %f, %f, %f\nAcceleration(x ,y, z): %f, %f, %f\n\n",
			i + 1,
			nBodyMass[i],
			nBodyPosition[i].x, nBodyPosition[i].y,
			nBodyVelocity[i].x, nBodyVelocity[i].y,
			nBodyAcceleration[i].x, nBodyAcceleration[i].y);
	}
}

void compute(bool parallel)
{
	if (parallel) {
		double start, end, minParallel = 1e30;

		start = CycleTimer::currentSeconds();

		for (int i = 1; i < 1000; ++i)
		{
			updatePhysicsParallel(i * 100);
		}
		end = CycleTimer::currentSeconds();
		minParallel = end - start;
		//display_bodies();

		printf("Time Taken by Parallel implementation: %f ms\n", (minParallel) * 1000);
	}
	else {
		double start, end, minParallel = 1e30;

		start = CycleTimer::currentSeconds();

		for (int i = 1; i < 1000; ++i)
		{
			updatePhysics(i * 100);
		}
		end = CycleTimer::currentSeconds();
		minParallel = end - start;
		//display_bodies();

		printf("Time Taken by serial implementation: %f ms\n", (minParallel) * 1000);
	}
	
}

// Physics

void updateAcceleration(int bodyIndex)
{

	Force2D netForce = { 0, 0 };

	for (int i = 0; i < BODY_COUNT; i++)
	{
		if (i == bodyIndex)
		{
			continue;
		}

		Force2D vectorForceToOther = { 0, 0 };
		Force scalarForceBetween = forceNewtonianGravity2D(
			nBodyMass[bodyIndex],
			nBodyMass[i],
			nBodyPosition[bodyIndex],
			nBodyPosition[i]
		);

		direction(
			nBodyPosition[bodyIndex],
			nBodyPosition[i],
			vectorForceToOther
		);

		vectorForceToOther.x *= scalarForceBetween;
		vectorForceToOther.y *= scalarForceBetween;
		netForce.x += vectorForceToOther.x;
		netForce.y += vectorForceToOther.y;
	}

	nBodyAcceleration[bodyIndex] = computeAccel2D(nBodyMass[bodyIndex], netForce);
}

void updateVelocity(int bodyIndex, float deltaT)
{
	nBodyVelocity[bodyIndex] = computeVelo2D(
		nBodyAcceleration[bodyIndex],
		nBodyVelocity[bodyIndex],
		deltaT);
}

void updatePosition(int bodyIndex, float deltaT)
{
	nBodyPosition[bodyIndex] = computePos2D(
		nBodyVelocity[bodyIndex],
		nBodyPosition[bodyIndex],
		deltaT);
}

void updatePhysicsParallel(float deltaT)
{
#pragma omp parallel for num_threads(omp_get_max_threads())
	for (int i = 0; i < BODY_COUNT; i++)
	{
		//printf("done: %d\n", i);
		updateAcceleration(i);
		updateVelocity(i, deltaT);
		updatePosition(i, deltaT);
	}
}

void updatePhysics(float deltaT)
{
	for (int i = 0; i < BODY_COUNT; i++)
	{
		//printf("done: %d\n", i);
		updateAcceleration(i);
		updateVelocity(i, deltaT);
		updatePosition(i, deltaT);
	}
}


int main()
{
	compute(true);
	compute(false);
	return 0;
}