/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Math2D.c
Purpose: This file contains the implementation for a library of 2D collision functions
Language: C
Platform: Windows
Project: CS529_twalton_Project3_Part2
Author: Tommy Walton, t.walton, 130000812
Creation date: 9/16/2013
- End Header -----------------------------------------------------*/

#include "Math2D.h"
#include "stdio.h"
#define EPSILON 0.000001f
/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(const Vector2D &P, const Vector2D &Center, float Radius)
{
    // Collision if the distance between pP and pCenter <= Radius, otherwise no collision
    if(Vector2DDistance(P, Center) <= Radius) return 1;
    else return 0;
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(const Vector2D &Pos, const Vector2D &Rect, float Width, float Height)
{
    if(Pos.x < Rect.x - Width * 0.5f //P.X < left
        || Pos.x > Rect.x + Width * 0.5f //P.X > right
        || Pos.y < Rect.y - Height * 0.5f //P.Y < bottom
        || Pos.y > Rect.y + Height * 0.5f) //P.Y > top
    {
        return 0;
    }
    else return 1;
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(const Vector2D &Center0, float Radius0, const Vector2D &Center1, float Radius1)
{
    if(Vector2DSquareDistance(Center0, Center1) <= (Radius0 + Radius1) * (Radius0 + Radius1))
    {
        return 1;
    }
    else return 0;
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(const Vector2D &Rect0, float Width0, float Height0, const Vector2D &Rect1, float Width1, float Height1)
{
    if(Rect0.x - Width0 * 0.5f >= Rect1.x + Width1 * 0.5f //left0 > right1
        || Rect1.x - Width1 * 0.5f >= Rect0.x + Width0 * 0.5f //left1 > right0
        || Rect0.y + Height0 * 0.5f <= Rect1.y - Height1 * 0.5f //top0 < bottom1
        || Rect1.y + Height1 * 0.5f <= Rect0.y - Height0 * 0.5f) // top1 < bottom0
    {
        return 0;
    }
    else return 1;
}


//////////////////////
// New to Project 3 //
//////////////////////

/*
This function checks if a static circle is intersecting with a static rectangle

Circle:		Center is "Center", radius is "Radius"
Rectangle:	Center is "Rect", width is "Width" and height is "Height"
Function returns true is the circle and rectangle are intersecting, otherwise it returns false
*/

int StaticCircleToStaticRectangle(const Vector2D &Center, float Radius, const Vector2D &Rect, float Width, float Height)
{
    // Set point to the center of the circle
    Vector2D point;
    point = Center;

    // Snap the center of the circle to the boundaries of the rectangle to find
    // the point on the rectangle that is closest to the circle

    // If the point is to the left of the hero
    if (point.x < Rect.x - Width * 0.5f)
        point.x = Rect.x - Width * 0.5f;
    // If the point is to the right of the hero
    if (point.x > Rect.x + Width * 0.5f)
        point.x = Rect.x + Width * 0.5f;
    // If the point is above the hero
    if (point.y > Rect.y + Height * 0.5f)
        point.y = Rect.y + Height * 0.5f;
    // If the point is below the hero
    if (point.y < Rect.y - Height * 0.5f)
        point.y = Rect.y - Height * 0.5f;

    return StaticPointToStaticCircle(point, Center, Radius);
}


//////////////////////
// New to project 4 //
//////////////////////

// Helper function
void Calculate_e_n_nHat(Vector2D &e, Vector2D &n, Vector2D &nHat, const Vector2D &P0, const Vector2D &P1)
{
    // e = P1 - P0
    Vector2DSub(e, P1, P0);
    
    // n = e rotated -90 degrees
    n.x = e.y;
    n.y = -e.x;

    // nHat = unit normal
    Vector2DNormalize(nHat, n);
}

/*
This function determines the distance separating a point from a line

 - Parameters
	- P:		The point whose location should be determined
	- LS:		The line segment

 - Returned value: The distance. Note that the returned value should be:
	- Negative if the point is in the line's inside half plane
	- Positive if the point is in the line's outside half plane
	- Zero if the point is on the line
*/
float StaticPointToStaticLineSegment(const Vector2D &P, LineSegment2D &LS)
{    
    Vector2D e;
    Vector2D n;
    Vector2D nHat;
    // Calculate L's edge vector and normal
    Calculate_e_n_nHat(e, n, nHat, LS.m_P0, LS.m_P1);

    return Vector2DDotProduct(nHat, P) - Vector2DDotProduct(nHat, LS.m_P0);
}

/*
This function determines if a circle intersects with a line segment

- Parameters
- P:		The center of the circle
- Radius:   The radius of the circle
- LS:		The line segment

- Returned value: 1 if they intersect, 0 otherwise
*/
int StaticCircleToStaticLineSegment(const Vector2D &CP, float Radius, LineSegment2D &LS)
{
    // Determine distance from center of circle to line
    float d = StaticPointToStaticLineSegment(CP, LS);

    if (-Radius <= d && d <= Radius)
        return 1;
    else
        return 0;
}

/*
This function checks whether an animated point is colliding with a line segment

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticLineSegment(const Vector2D &Ps, const Vector2D &Pe, LineSegment2D &LS, Vector2D &Pi)
{
    // Test for non-collision
    Vector2D e; // LS edge vector
    Vector2D n; // LS normal
    Vector2D nHat; // LS unit normal
    Vector2D v; // Velocity of point P
    float ti; // Time of intersection
    Vector2D temp;

    // Calculate L's edge vector and normal
    Calculate_e_n_nHat(e, n, nHat, LS.m_P0, LS.m_P1);
    // Calculate the point's velocity
    Vector2DSub(v, Pe, Ps);

    if( StaticPointToStaticLineSegment(Ps, LS) < 0 &&
        StaticPointToStaticLineSegment(Pe, LS) < 0 )
    {
        // No collision: both start and end points are on inside half-plane
        return -1.0f;
    }
    if( StaticPointToStaticLineSegment(Ps, LS) > 0 &&
        StaticPointToStaticLineSegment(Pe, LS) > 0 )
    {
        // No collision: both start and end points are on outside half-plane
        return -1.0f;
    }
    // If nHat.v == 0, but with some room for floating point error
    if(Vector2DDotProduct(nHat, v) < EPSILON &&
        Vector2DDotProduct(nHat, v) > -EPSILON)
    {
        // No collision: Pe - Ps is parallel to the line segment
        return -1.0f;
    }

    // Calculate Pi
    // ti = ((n.P0 - n.Bs)/n.v)
    ti = (Vector2DDotProduct(nHat, LS.m_P0) - Vector2DDotProduct(nHat, Ps)) / Vector2DDotProduct(nHat, v);
    
    // ti must be between 0 and 1 for a collision
    if(ti < 0 || ti > 1)
    {
        return -1.0f;
    }

    // Pi = Ps + v*ti
    Vector2DScaleAdd(Pi, v, Ps, ti);
    
    // Two more tests for non-collision
    // Temp = Pi - P0
    Vector2DSub(temp, Pi, LS.m_P0);
    //(Pi - P0).(P1 - P0) < 0
    if(Vector2DDotProduct(temp, e) < 0)
    {
        // No collision: ball collides with infinite extension of wall, not finite wall
        return -1.0f;
    }
    // Temp = Pi - P1
    Vector2DSub(temp, Pi, LS.m_P1);
    // Flipping the direction of v from P1 - P0 to P0 - P1 (v itself is not needed anymore)
    Vector2DSub(v, LS.m_P0, LS.m_P1);
    //(Pi - P1).(P0 - P1) < 0
    if(Vector2DDotProduct(temp, v) < 0)
    {
        // No collision: ball collides with infinite extension of wall, not finite wall
        return -1.0f;
    }

    return ti;
}


/*
This function checks whether an animated circle is colliding with a line segment

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticLineSegment(const Vector2D &Ps, const Vector2D &Pe, float Radius, LineSegment2D &LS, Vector2D &Pi)
{
    Vector2D e;
    Vector2D n;
    Vector2D nHat;
    float D;
    Vector2D CPs; // start position of the point on the circle that is closest to LS
    Vector2D CPe; // end position of the point on the circle that is closest to LS

    Calculate_e_n_nHat(e, n, nHat, LS.m_P0, LS.m_P1);
    
    // Determine which radius to use
    // D = -r when Ps is inside, r when Ps is outside
    if(StaticPointToStaticLineSegment(Ps, LS) < 0)
        D = -Radius;
    else
        D = Radius;

    // Calculate the point on the circle that is closest to the line
    // Start at the center of the circle, travel in the direction of nHat for distance D
    // CPs = Ps + nHat * D
    Vector2DScaleAdd(CPs, nHat, Ps, D);
    
    // Calculate the end position of CPs
    // CPe = CPs + Pe - Ps
    Vector2DSub(CPe, LS.m_P1, LS.m_P0);
    // Call AnimatedPointToStaticLineSegment
    return AnimatedPointToStaticLineSegment(CPs, CPe, LS, Pi);
    
    // Note: If part of the circle that was not initially closest to the line
    // collides with an endpoint of the line segment
    // this function will reject it as no intersection.
}


/*
This function reflects an animated point on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticLineSegment(const Vector2D &Ps, const Vector2D &Pe, LineSegment2D &LS, Vector2D &Pi, Vector2D &R)
{
    Vector2D e, n, nHat, i, s, m, r, Pe_;
    float ti;

    Calculate_e_n_nHat(e, n, nHat, LS.m_P0, LS.m_P1);

    ti = AnimatedPointToStaticLineSegment(Ps, Pe, LS, Pi);
    if (ti == -1.0f) return -1.0f;

    // i = Pe - Pi
    Vector2DSub(i, Pe, Pi);
    // s = (i.nHat)nHat
    Vector2DScale(s, nHat, Vector2DDotProduct(i, nHat));
    // m = i - s
    Vector2DSub(m, i, s);
    // r = m - s
    Vector2DSub(r, m, s);
    // Pe' = Pi + r
    Vector2DAdd(Pe_, Pi, r);
    // R = Pe_ - Pi
    Vector2DSub(R, Pe_, Pi);

    return ti;
}


/*
This function reflects an animated circle on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticLineSegment(const Vector2D &Ps, const Vector2D &Pe, float Radius, LineSegment2D &LS, Vector2D &Pi, Vector2D &R)
{
    Vector2D e;
    Vector2D n;
    Vector2D nHat;
    float D;
    Vector2D CPs; // start position of the point on the circle that is closest to LS
    Vector2D CPe; // end position of the point on the circle that is closest to LS
    float ret;

    Calculate_e_n_nHat(e, n, nHat, LS.m_P0, LS.m_P1);
    
    // Determine which radius to use
    // D = -r when Ps is inside, r when Ps is outside
    if(StaticPointToStaticLineSegment(Ps, LS) < 0)
        D = Radius;
    else
        D = -Radius;

    // Calculate the point on the circle that is closest to the line
    // Start at the center of the circle, travel in the direction of nHat for distance D
    // CPs = Ps + nHat * D
    Vector2DScaleAdd(CPs, nHat, Ps, D);
    
    // Calculate the end position of CPs
    // CPe = CPs + Pe - Ps
    Vector2DSub(CPe, Pe, Ps);
    Vector2DAdd(CPe, CPe, CPs);
    // Call ReflectAnimatedPointToStaticLineSegment
    ret = ReflectAnimatedPointOnStaticLineSegment(CPs, CPe, LS, Pi, R);
    // Adjust Pi by R
    Vector2DScaleAdd(Pi, nHat, Pi, -D);
    return ret;
    // Note: Same pitfall as AnimatedCircleToStaticLineSegment:    
    // If part of the circle that was not initially closest to the line
    // collides with an endpoint of the line segment
    // this function will reject it as no intersection.
}


/*
This function checks whether an animated point is colliding with a static circle

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticCircle(const Vector2D &Ps, const Vector2D &Pe, const Vector2D &Center, float Radius, Vector2D &Pi)
{
    // Compute projection pof Center - Ps onto Pe - Ps
    // m = (Center - Ps).(Pe - Ps / ||Pe - Ps||)
    Vector2D PsC, PsPe, PsPeHat;
    float m, nSquared, ti0, ti1, ti, a, b, c;
    Vector2DSub(PsC, Center, Ps);
    Vector2DSub(PsPe, Pe, Ps);
    Vector2DNormalize(PsPeHat, PsPe);
    m = Vector2DDotProduct(PsC, PsPeHat);

    // Test for non-collision
    if (m < 0) // Ps is outside circle -> circle behind ray origin
        return -1.0f;

    // n^2 = ||PsC||^2 - m^2
    nSquared = Vector2DSquareLength(PsC) - m * m;
    if (nSquared > Radius * Radius) // ray will miss the circle
        return -1.0f;

    // Otherwise compute ti
    // a = PsPe.PsPe, b = -2(PsC).v, c = PsC.PsC - r^2
    a = Vector2DDotProduct(PsPe, PsPe);
    b = -2.0f * Vector2DDotProduct(PsC, PsPe);
    c = Vector2DDotProduct(PsC, PsC) - (Radius * Radius);

    // If b^2 - 4ac < 0 -> ray misses circle
    if ((b * b - (4.0f * a * c)) < 0)
        return 1.0f;

    // Solve for t using the quadratic equation
    ti0 = (-b - (float)sqrt(b * b - (4.0f * a * c))) / (2.0f * a);
    ti1 = (-b + (float)sqrt(b * b - (4.0f * a * c))) / (2.0f * a);

    ti = (ti0 < ti1 ? ti0 : ti1);

    if (ti > 1.0f)
        return -1.0f;

    // Calculate Pi
    Vector2DScaleAdd(Pi, PsPe, Ps, ti);

    return ti;
}



/*
This function reflects an animated point on a static circle.
It should first make sure that the animated point is intersecting with the circle 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticCircle(const Vector2D &Ps, const Vector2D &Pe, const Vector2D &Center, float Radius, Vector2D &Pi, Vector2D &R)
{
    // Compute Pi, the intersection point
    Vector2D n, nHat, m, u, PsPe;
    float ti = AnimatedPointToStaticCircle(Ps, Pe, Center, Radius, Pi);

    // If there is no intersection, return ti
    if (ti < 0)
        return ti;

    // Calculate n and nHat
    Vector2DSub(n, Center, Pi);
    Vector2DNormalize(nHat, n);

    // Calculate m = Ps - Pi
    Vector2DSub(m, Ps, Pi);

    // Calculate u = 2(m.nHat)nHat
    Vector2DScale(u, nHat, 2.0f * Vector2DDotProduct(m, nHat));

    // Calculate R = 2(m.nHat)nHat - m = u - m
    Vector2DSub(R, u, m);

    // Now scale R to be the length of the original vector PsPe * (1 - ti)
    Vector2DSub(PsPe, Pe, Ps);
    Vector2DNormalize(R, R);
    Vector2DScale(R, R, (1.0f - ti) * Vector2DLength(PsPe));

    return ti;
}


/*
This function checks whether an animated circle is colliding with a static circle

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticCircle(const Vector2D &Center0s, const Vector2D &Center0e, float Radius0, const Vector2D &Center1, float Radius1, Vector2D &Pi)
{
    return AnimatedPointToStaticCircle(Center0s, Center0e, Center1, Radius0 + Radius1, Pi);
}


/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one 

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:			Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticCircle(const Vector2D &Center0s, const Vector2D &Center0e, float Radius0, const Vector2D &Center1, float Radius1, Vector2D &Pi, Vector2D &R)
{
    return ReflectAnimatedPointOnStaticCircle(Center0s, Center0e, Center1, Radius0 + Radius1, Pi, R);
}

/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one

- Parameters
    - Center0s:		The starting position of the reflecting circle's center
    - Center0e:		The ending position of the reflecting circle's center
    - Radius0:		The reflecting circle's radius
    - Center1s:		The starting position of the colliding circle's center
    - Center1e:     The ending position of the colliding circle's center
    - Radius1:		The colliding circle's radius
    - Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)
    - R:			Reflected vector R

- Returned value: Intersection time t
- -1.0f:		If there's no intersection
- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnAnimatedCircle(const Vector2D &Center0s, const Vector2D &Center0e, float Radius0, const Vector2D &Center1s, const Vector2D &Center1e, float Radius1, Vector2D &Pi, Vector2D &R)
{
    //
    // First get the position of the colliding circle at the point of impact
    //

    // Subtract the velocity of Circle1 from Circle0 to result in an animated circle to static circle collision
    Vector2D CollidingCircleVelocity;
    Vector2DSub(CollidingCircleVelocity, Center1e, Center1s);
    Vector2D tempCenterEnd;
    Vector2DSub(tempCenterEnd, Center0e, CollidingCircleVelocity);

    // Now do a collision between the animated reflecting circle and static colliding circle to get a time of impact
    float ti = ReflectAnimatedCircleOnStaticCircle(
        Center0s,
        tempCenterEnd,
        Radius0, 
        Center1s,
        Radius1,
        Pi,
        R
        );
    // If the reflecting circle has a non-zero velocity
    if (Center0s.x != Center0e.x || Center0s.y != Center0e.y)
    {
        // Calculate the position of the colliding circle at the point of impact
        Vector2D CollidingCircleCenterAtImpact = Center1s;
        CollidingCircleCenterAtImpact.x += (Center1s.x - Center1e.x) * ti;
        CollidingCircleCenterAtImpact.y += (Center1s.y - Center1e.y) * ti;

        //
        // Now do a animated circle to static circle reflection
        //
        ReflectAnimatedCircleOnStaticCircle(
            Center0s,
            Center0e,
            Radius0,
            CollidingCircleCenterAtImpact,
            Radius1,
            Pi,
            R
            );
    }
    else
    {

        //
        // Now that the time of impact and reflection vector have been calculated, calculate the proper point of impact
        //

        // Calculate the position of the colliding circle at the point of impact
        Vector2D Center0i = Center0s;
        Center0i.x += (Center0s.x - Center0e.x) * ti;
        Center0i.y += (Center0s.y - Center0e.y) * ti;
        // Calculate the position of the colliding circle at the point of impact
        Vector2D Center1i = Center1s;
        Center1i.x += (Center1s.x - Center1e.x) * ti;
        Center1i.y += (Center1s.y - Center1e.y) * ti;

        // Halfway between the two center points at the time of impact
        Pi.x = (Center0i.x + Center1i.x) * 0.5f;
        Pi.y = (Center0i.y + Center1i.y) * 0.5f;
    }
    

    return ti;
}