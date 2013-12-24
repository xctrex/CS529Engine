/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D.h
Purpose: This is a header file for a 2D vector library
Language: C
Platform: Windows
Project: CS529_twalton_Project2_Part2
Author: Tommy Walton, t.walton, 130000812
Creation date: 9/16/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "math.h"
#include <DirectXMath.h>

#ifndef PI
#define PI 3.14159265f
#endif

/*

typedef struct Vector2D
{
	float x, y;
}Vector2D;
*/
#ifndef Vector2D
#define Vector2D DirectX::XMFLOAT2
#endif
/*
This function converts degrees to radians
*/
float DegreesToRadians(float degrees);
/*
This function converts radians to degrees
*/
float RadiansToDegrees(float radians);

/*
This function sets the coordinates of the 2D vector (pResult) to 0
*/
void Vector2DZero(Vector2D &Result);

/*
This function sets the coordinates of the 2D vector (pResult) to x &y
*/
void Vector2DSet(Vector2D &Result, float x, float y);

/*
In this function, pResult will be set to the opposite of pVec0
*/
void Vector2DNeg(Vector2D &Result, const Vector2D &Vec0);

/*
In this function, pResult will be the sum of pVec0 and pVec1
*/
void Vector2DAdd(Vector2D &Result, const Vector2D &Vec0, const Vector2D &Vec1);

/*
In this function, pResult will be the difference between pVec0 and pVec1: pVec0 - pVec1
*/
void Vector2DSub(Vector2D &Result, const Vector2D &Vec0, const Vector2D &Vec1);

/*
In this function, pResult will be the unit vector of pVec0
*/
void Vector2DNormalize(Vector2D &Result, const Vector2D &Vec0);

/*
In this function, pResult will be the vector pVec0 scaled by the value c
*/
void Vector2DScale(Vector2D &Result, const Vector2D &Vec0, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and added to pVec1 
*/
void Vector2DScaleAdd(Vector2D &Result, const Vector2D &Vec0, const Vector2D &Vec1, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and pVec1 will be subtracted from it 
*/
void Vector2DScaleSub(Vector2D &Result, const Vector2D &Vec0, const Vector2D &Vec1, float c);

/*
This function returns the length of the vector pVec0
*/
float Vector2DLength(const Vector2D &Vec0);

/*
This function returns the square of pVec0's length. Avoid the square root
*/
float Vector2DSquareLength(const Vector2D &Vec0);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The distance between these 2 2D points is returned
*/
float Vector2DDistance(const Vector2D &Vec0, const Vector2D &Vec1);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The squared distance between these 2 2D points is returned. Avoid the square root
*/
float Vector2DSquareDistance(const Vector2D &Vec0, const Vector2D &Vec1);

/*
This function returns the dot product between pVec0 and pVec1
*/
float Vector2DDotProduct(const Vector2D &Vec0, const Vector2D &Vec1);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Degrees
*/
void Vector2DFromAngleDeg(Vector2D &Result, float angle);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Radian
*/
void Vector2DFromAngleRad(Vector2D &Result, float angle);

/*
This function computes the angle in degrees of a vector
*/
float AngleDegFromVector2D(const Vector2D &vector);

/*
This function computes the angle in radians of a vector
*/
float AngleRadFromVector2D(const Vector2D &vector);
