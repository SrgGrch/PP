#ifndef VECTORMATH_H_
#define VECTORMATH_H_

#include <iostream>
#include <cmath>

// Constants
const static double PI = 3.141592653589f;
const static double G = 6.67e-11f;

// Vectors
typedef double Vector;
typedef Vector Force;
typedef Vector Acceleration;
typedef Vector Position;
typedef Vector Velocity;

typedef struct Vector2D 
{
  Vector x;
  Vector y;
};

typedef Vector2D Force2D;
typedef Vector2D Acceleration2D;
typedef Vector2D Velocity2D;
typedef Vector2D Position2D;

// Scalar
typedef double Scalar;
typedef Scalar Mass;
typedef Scalar Time;

// Vector operations
inline Scalar magnitude(const Vector2D &aVector) 
{
  Scalar squareOfLength = 0.0;
  squareOfLength += aVector.x * aVector.x;
  squareOfLength += aVector.y * aVector.y;
  return sqrt( squareOfLength );
}

inline void normalize(Vector2D &aVector) 
{
  Scalar length = magnitude(aVector);
  aVector.x = aVector.x / length;
  aVector.y = aVector.y / length;
}

inline void invert(Vector2D &aVector) 
{
  aVector.x *= -1.0;
  aVector.y *= -1.0;
}

inline void direction(
        const Vector2D &fromVector,
        const Vector2D &toVector,
        Vector2D &resultVector) 
{
  resultVector.x = toVector.x - fromVector.x;
  resultVector.y = toVector.y - fromVector.y;
  normalize( resultVector );
}

// Physics operations

inline Force forceNewtonianGravity2D( 
          Mass onMass, 
          Mass becauseOfMass,
          Position2D onPosition, 
          Position2D becauseOfPosition) 
{
  Scalar deltaX = becauseOfPosition.x - onPosition.x;
  Scalar deltaY = becauseOfPosition.y - onPosition.y;
  Scalar distance = sqrt(deltaX * deltaX + deltaY * deltaY);

  if( distance == 0 ) 
  {
    return 0;
  }

  Force result = G * (onMass * becauseOfMass) /  (distance * distance);
  return result;
}

inline Acceleration computeAccel( Mass mass, Force force ) 
{
  if( force == 0 ) 
  {
    return 0;
  }

  Scalar result = force / mass;
  return result;
}

inline Velocity computeVelo(Acceleration current, Velocity previous, Time deltaT) 
{
  return previous + (current * deltaT);
}

inline Position computePos(Velocity current, Position previous, Time deltaT) 
{
  return previous + (current * deltaT);
}

inline Acceleration2D computeAccel2D(Mass mass, const Force2D &force) 
{
  Acceleration2D anAccelVector = {0, 0};
  anAccelVector.x = computeAccel(mass, force.x);
  anAccelVector.y = computeAccel(mass, force.y);
  return anAccelVector;
}

inline Velocity2D computeVelo2D(Acceleration2D &accel, Velocity2D &prevVelo, Time deltaT) 
{
  Velocity2D aVelocityVector = {0, 0};
  aVelocityVector.x = computeVelo( accel.x, prevVelo.x, deltaT );
  aVelocityVector.y = computeVelo( accel.y, prevVelo.y, deltaT );
  return aVelocityVector;
}

inline Position2D computePos2D(Velocity2D &velo, Position2D &prevPos, Time deltaT) 
{
  Position2D anPositionVector = {0, 0};
  anPositionVector.x = computePos(velo.x, prevPos.x, deltaT);
  anPositionVector.y = computePos(velo.y, prevPos.y, deltaT);
  return anPositionVector;
}

inline int func()
{
  return 5;

}

#endif
