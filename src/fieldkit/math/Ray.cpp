/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 26/07/2010.
 */

#include "fieldkit/math/Ray.h"

#include "fieldkit/math/Line.h"

using namespace fieldkit::math;

//! Calculates the intersection between two rays and checks for parallel rays.
bool Ray2f::findIntersection(Ray2f ray, Vec2f* result)
{	
	Vec2f p1 = origin;
	Vec2f p2 = direction;
	
	Vec2f p3 = ray.origin;
	Vec2f p4 = ray.direction;
	
	float xD1, yD1, xD2, yD2, xD3, yD3;
	float dot, deg, len1, len2;
	float ua, div;
	
	// calculate differences
	xD1 = p2.x - p1.x;
	xD2 = p4.x - p3.x;
	yD1 = p2.y - p1.y;
	yD2 = p4.y - p3.y;
	xD3 = p1.x - p3.x;
	yD3 = p1.y - p3.y;
	
	// calculate the lengths of the two lines
	len1 = sqrt(xD1 * xD1 + yD1 * yD1);
	len2 = sqrt(xD2 * xD2 + yD2 * yD2);
	
	// calculate angle between the two lines.
	dot = (xD1 * xD2 + yD1 * yD2); // dot product
	deg = dot / (len1 * len2);
	
	// if abs(angle) = = 1 then the lines are parallel,
	// so no intersection is possible
	if(abs(deg) == 1)
		return false;
	
	// find intersection Pt between two lines
	div = yD2 * xD1 - xD2 * yD1;
	ua = (xD2 * yD3 - yD2 * xD3) / div;

	result->x = p1.x + ua * xD1;
	result->y = p1.y + ua * yD1;
	return true;
}


//! Calculates the intersection between a ray and a line, also checks for parallel lines.
bool Ray2f::findIntersection(Line2f line, Vec2f* result)
{	
	Vec2f p1 = origin;
	Vec2f p2 = origin + direction;
	
	Vec2f p3 = line.p1;
	Vec2f p4 = line.p2;
	
	float xD1, yD1, xD2, yD2, xD3, yD3;
	float dot, deg, len1, len2;
	float segmentLen;
	float ua, div;
	
	// calculate differences
	xD1 = p2.x - p1.x;
	xD2 = p4.x - p3.x;
	yD1 = p2.y - p1.y;
	yD2 = p4.y - p3.y;
	xD3 = p1.x - p3.x;
	yD3 = p1.y - p3.y;
	
	// calculate the lengths of the two lines
	len1 = sqrt(xD1 * xD1 + yD1 * yD1);
	len2 = sqrt(xD2 * xD2 + yD2 * yD2);
	
	// calculate angle between the two lines.
	dot = (xD1 * xD2 + yD1 * yD2); // dot product
	deg = dot / (len1 * len2);
	
	// if abs(angle) = = 1 then the lines are parallel,
	// so no intersection is possible
	if(abs(deg) == 1)
		return false;
	
	// find intersection Pt between two lines
	div = yD2 * xD1 - xD2 * yD1;
	ua = (xD2 * yD3 - yD2 * xD3) / div;
	
	Vec2f pt = Vec2f(p1.x + ua * xD1, p1.y + ua * yD1);
	
	// calculate the combined length of the two segments
	// between Pt - p3 and Pt - p4
	xD1 = pt.x - p3.x;
	xD2 = pt.x - p4.x;
	yD1 = pt.y - p3.y;
	yD2 = pt.y - p4.y;
	segmentLen = sqrt(xD1 * xD1 + yD1 * yD1)
			   + sqrt(xD2 * xD2 + yD2 * yD2);
	
	// if the lengths of both sets of segments are the same as
	// the lenghts of the two lines the point is actually
	// on the line segment.
	// if the point isn't on the line , return false
	if(abs(len2 - segmentLen) > 0.01f)
		return false;
	
	// return valid intersection
	result->x = pt.x;
	result->y = pt.y;
	return true;
}

