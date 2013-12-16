/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: LineSegment2D.h
Purpose: Header file for line segment math library
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Vector2D.h"



typedef struct LineSegment2D
{
	Vector2D m_P0;		// Point on the line
	Vector2D m_P1;		// Point on the line
	Vector2D m_N;		// Line's normal
	float m_NdotP0;		// To avoid computing it every time it's needed
}LineSegment2D;


/*
This function builds a 2D line segment's data using 2 points
 - Computes the normal (Unit Vector)
 - Computes the dot product of the normal with one of the points

 - Parameters
	- LS:		The to-be-built line segment
	- Point0:	One point on the line
	- Point1:	Another point on the line

 - Returns 1 if the line equation was built successfully 
*/
int BuildLineSegment2D(LineSegment2D &LS, const Vector2D &Point0, const Vector2D &Point1);




