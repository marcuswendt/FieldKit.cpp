/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 26/07/2010.
 */

#include "fieldkit/math/Line.h"

#include "fieldkit/math/Ray.h"

using namespace fieldkit::math;

void Line2f::set(Vec2f from, Vec2f to)
{
	p1 = from;
	p2 = to;
}

//! Calculates intersection and checks for parallel lines.
//! Also checks that the intersection point is actually on the line segment p1 - p2
bool Line2f::findIntersection(Line2f const& line, Vec2f* result)
{	
	Vec2f p1 = this->p1;
	Vec2f p2 = this->p2;
	
	Vec2f p3 = line.p1;
	Vec2f p4 = line.p2;
	
	float xD1, yD1, xD2, yD2, xD3, yD3;
	float dot, deg, len1, len2;
	float segmentLen1, segmentLen2;
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
	// between Pt - p1 and Pt - p2
	xD1 = pt.x - p1.x;
	xD2 = pt.x - p2.x;
	yD1 = pt.y - p1.y;
	yD2 = pt.y - p2.y;
	segmentLen1 = sqrt(xD1 * xD1 + yD1 * yD1)
				+ sqrt(xD2 * xD2 + yD2 * yD2);
	
	// calculate the combined length of the two segments
	// between Pt - p3 and Pt - p4
	xD1 = pt.x - p3.x;
	xD2 = pt.x - p4.x;
	yD1 = pt.y - p3.y;
	yD2 = pt.y - p4.y;
	segmentLen2 = sqrt(xD1 * xD1 + yD1 * yD1)
				+ sqrt(xD2 * xD2 + yD2 * yD2);
	
	// if the lengths of both sets of segments are the same as
	// the lenghts of the two lines the point is actually
	// on the line segment.
	// if the point isn't on the line , return false
	if (abs(len1 - segmentLen1) > 0.01f || 
		abs(len2 - segmentLen2) > 0.01f)
		return false;
	
	// return valid intersection
	if(result != NULL) {
		result->x = pt.x;
		result->y = pt.y;
	}

	return true;
}


////! Calculates the intersection between a ray and a line, also checks for parallel lines.
//bool Line2f::findIntersection(Ray2f const& ray, Vec2f* result)
//{
//	return ray.findIntersection(this, result);
//}
