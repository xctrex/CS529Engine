/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Math2D.h
Purpose: This is the header file for a library of 2D collision functions
Language: C
Platform: Windows
Project: CS529_twalton_Project3_Part2
Author: Tommy Walton, t.walton, 130000812
Creation date: 9/16/2013
- End Header -----------------------------------------------------*/
#pragma once


#include "LineSegment2D.h"
#include "Matrix2D.h"

////////////////////////
// From Project 2 & 3 //
////////////////////////


/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(const Vector2D &P, const Vector2D &Center, float Radius);


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(const Vector2D &Pos, const Vector2D &Rect, float Width, float Height);

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(const Vector2D &pCenter0, float Radius0, const Vector2D &pCenter1, float Radius1);

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(const Vector2D &Rect0, float Width0, float Height0, const Vector2D &Rect1, float Width1, float Height1);


//////////////////////
// New to project 4 //
//////////////////////


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
float StaticPointToStaticLineSegment(const Vector2D &P, LineSegment2D &LS);

/*
This function determines if a circle intersects with a line segment

- Parameters
- P:		The center of the circle
- Radius:   The radius of the circle
- LS:		The line segment

- Returned value: 1 if they intersect, 0 otherwise
*/
int StaticCircleToStaticLineSegment(const Vector2D &CP, float Radius, LineSegment2D &LS);

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
float AnimatedPointToStaticLineSegment(const Vector2D &Ps, const Vector2D &Pe, LineSegment2D &LS, Vector2D &Pi);


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
float AnimatedCircleToStaticLineSegment(const Vector2D &Ps, const Vector2D &Pe, float Radius, LineSegment2D &LS, Vector2D &Pi);


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
float ReflectAnimatedPointOnStaticLineSegment(const Vector2D &Ps, const Vector2D &Pe, LineSegment2D &LS, Vector2D &Pi, Vector2D &R);


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
float ReflectAnimatedCircleOnStaticLineSegment(const Vector2D &Ps, const Vector2D &Pe, float Radius, LineSegment2D &LS, Vector2D &Pi, Vector2D &R);


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
float AnimatedPointToStaticCircle(const Vector2D &Ps, const Vector2D &Pe, const Vector2D &Center, float Radius, Vector2D &Pi);



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
float ReflectAnimatedPointOnStaticCircle(const Vector2D &Ps, const Vector2D &Pe, const Vector2D &Center, float Radius, Vector2D &Pi, Vector2D &R);


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
float AnimatedCircleToStaticCircle(const Vector2D &Center0s, const Vector2D &Center0e, float Radius0, const Vector2D &Center1, float Radius1, Vector2D &Pi);


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
float ReflectAnimatedCircleOnStaticCircle(const Vector2D &Center0s, const Vector2D &Center0e, float Radius0, const Vector2D &Center1, float Radius1, Vector2D &Pi, Vector2D &R);

/*
This function reflects an animated circle on an animated circle.
It should first make sure that the circles are intersecting

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
float ReflectAnimatedCircleOnAnimatedCircle(
    const Vector2D &Center0s,
    const Vector2D &Center0e,
    float Radius0,
    const Vector2D &Center1s,
    const Vector2D &Center1e,
    float Radius1,
    Vector2D &Pi,
    Vector2D &R
    );
