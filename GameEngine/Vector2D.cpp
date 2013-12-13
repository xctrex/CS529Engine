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

void Vector2DZero(Vector2D *pResult)
{
    pResult->x = 0;
    pResult->y = 0;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D *pResult, float x, float y)
{
    pResult->x = x;
    pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
    pResult->x = pVec0->x * -1;
    pResult->y = pVec0->y * -1;
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
    pResult->x = pVec0->x + pVec1->x;
    pResult->y = pVec0->y + pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
    pResult->x = pVec0->x - pVec1->x;
    pResult->y = pVec0->y - pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
    // Only create the temporary variable "length" if pResult == pVec0
    if(pResult == pVec0)
    {
        float length = Vector2DLength(pVec0);
        pResult->x = pVec0->x / length;
        pResult->y = pVec0->y / length;
    }
    else
    {
        pResult->x = pVec0->x / Vector2DLength(pVec0);
        pResult->y = pVec0->y / Vector2DLength(pVec0);
    }
}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
    pResult->x = pVec0->x * c;
    pResult->y = pVec0->y * c;
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
    Vector2D tempScale;
    Vector2DScale(&tempScale, pVec0, c);
    Vector2DAdd(pResult, &tempScale, pVec1);
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
    Vector2DScale(pResult, pVec0, c);
    Vector2DSub(pResult, pResult, pVec1);
}

// ---------------------------------------------------------------------------

float Vector2DLength(Vector2D *pVec0)
{
    return sqrt(pVec0->x * pVec0->x + pVec0->y * pVec0->y);
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(Vector2D *pVec0)
{
	return pVec0->x * pVec0->x + pVec0->y * pVec0->y;
}

// ---------------------------------------------------------------------------

float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
    return sqrt((pVec0->x - pVec1->x) * (pVec0->x - pVec1->x) + (pVec0->y - pVec1->y) * (pVec0->y - pVec1->y));
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return (pVec0->x - pVec1->x) * (pVec0->x - pVec1->x) + (pVec0->y - pVec1->y) * (pVec0->y - pVec1->y);
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
    return pVec0->x * pVec1->x + pVec0->y * pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
    Vector2DFromAngleRad(pResult, angle * PI / 180.0f);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
    pResult->x = cos(angle);
    pResult->y = sin(angle);
}

// ---------------------------------------------------------------------------
