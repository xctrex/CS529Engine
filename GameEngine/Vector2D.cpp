/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D.c
Purpose: This file contains the implementation of a 2D vector library
Language: C
Platform: Windows
Project: CS529_twalton_Project3_Part1
Author: Tommy Walton, t.walton, 130000812
Creation date: 9/16/2013
- End Header -----------------------------------------------------*/

#include "Vector2D.h"

#define EPSILON 0.00001


// ---------------------------------------------------------------------------

float DegreesToRadians(float degrees) { return degrees * PI / 180.0f; }

// ---------------------------------------------------------------------------

float RadiansToDegrees(float radians) { return radians * 180.0f / PI; }
// ---------------------------------------------------------------------------

void Vector2DZero(Vector2D &Result)
{
    Result.x = 0;
    Result.y = 0;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D &Result, float x, float y)
{
    Result.x = x;
    Result.y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D &Result, const Vector2D &Vec0)
{
    Result.x = Vec0.x * -1;
    Result.y = Vec0.y * -1;
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D &Result, const Vector2D &Vec0, const Vector2D &Vec1)
{
    Result.x = Vec0.x + Vec1.x;
    Result.y = Vec0.y + Vec1.y;
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D &Result, const Vector2D &Vec0, const Vector2D &Vec1)
{
    Result.x = Vec0.x - Vec1.x;
    Result.y = Vec0.y - Vec1.y;
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D &Result, const Vector2D &Vec0)
{
    float length = Vector2DLength(Vec0);
    Result.x = Vec0.x / length;
    Result.y = Vec0.y / length;
}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D &Result, const Vector2D &Vec0, float c)
{
    Result.x = Vec0.x * c;
    Result.y = Vec0.y * c;
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D &Result, const Vector2D &Vec0, const Vector2D &Vec1, float c)
{
    Vector2D tempScale;
    Vector2DScale(tempScale, Vec0, c);
    Vector2DAdd(Result, tempScale, Vec1);
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D &Result, const Vector2D &Vec0, const Vector2D &Vec1, float c)
{
    Vector2DScale(Result, Vec0, c);
    Vector2DSub(Result, Result, Vec1);
}

// ---------------------------------------------------------------------------

float Vector2DLength(const Vector2D &Vec0)
{
    return sqrt(Vec0.x * Vec0.x + Vec0.y * Vec0.y);
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(const Vector2D &Vec0)
{
	return Vec0.x * Vec0.x + Vec0.y * Vec0.y;
}

// ---------------------------------------------------------------------------

float Vector2DDistance(const Vector2D &Vec0, const Vector2D &Vec1)
{
    return sqrt((Vec0.x - Vec1.x) * (Vec0.x - Vec1.x) + (Vec0.y - Vec1.y) * (Vec0.y - Vec1.y));
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(const Vector2D &Vec0, const Vector2D &Vec1)
{
	return (Vec0.x - Vec1.x) * (Vec0.x - Vec1.x) + (Vec0.y - Vec1.y) * (Vec0.y - Vec1.y);
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(const Vector2D &Vec0, const Vector2D &Vec1)
{
    return Vec0.x * Vec1.x + Vec0.y * Vec1.y;
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D &Result, float angle)
{
    Vector2DFromAngleRad(Result, angle * PI / 180.0f);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D &Result, float angle)
{
    Result.x = cos(angle);
    Result.y = sin(angle);
}

// ---------------------------------------------------------------------------
