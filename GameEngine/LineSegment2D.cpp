#include "LineSegment2D.h"

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
int BuildLineSegment2D(LineSegment2D &LS, const Vector2D &Point0, const Vector2D &Point1)
{
    Vector2D e;
    Vector2DSet(LS.m_P0, Point0.x, Point0.y); // Point on the line
	Vector2DSet(LS.m_P1, Point1.x, Point1.y); // Point on the line

    // Calculate the normal
	// e = P1 - P0
    Vector2DSub(e, Point1, Point0);
    
    // n = e rotated -90 degrees
    LS.m_N.x = e.y;
    LS.m_N.y = -e.x;

    LS.m_NdotP0 = Vector2DDotProduct(LS.m_N, Point0); // To avoid computing it every time it's needed
    return 1;
}