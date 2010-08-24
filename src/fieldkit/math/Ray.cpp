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
bool Ray2f::findIntersection(Ray2f const& ray, Vec2f* result)
{	
	Vec2f p1 = origin;
	Vec2f p2 = origin + direction;
	
	Vec2f p3 = ray.origin;
	Vec2f p4 = ray.origin + ray.direction;
	
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

	// return valid intersection if requested
	if(result != NULL) {
		result->x = p1.x + ua * xD1;
		result->y = p1.y + ua * yD1;
	}
	
	return true;
}

//! Calculates the intersection between a ray and a line, also checks for parallel lines.
//! based on Paul Bourke's "Intersection Point Of Two Lines (2 Dimensions)"
//! http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
bool Ray2f::findIntersection(Line2f const& line, Vec2f* result)
{	
	Vec2f p1 = origin;
	Vec2f p2 = origin + direction;

	float denom = ((line.p2.y - line.p1.y) * (p2.x - p1.x)) -
			 	  ((line.p2.x - line.p1.x) * (p2.y - p1.y));

	float numea = ((line.p2.x - line.p1.x) * (p1.y - line.p1.y)) -
				  ((line.p2.y - line.p1.y) * (p1.x - line.p1.x));

	float numeb = ((p2.x - p1.x) * (p1.y - line.p1.y)) -
				  ((p2.y - p1.y) * (p1.x - line.p1.x));

	if(denom == 0.0f) {
		if(numea == 0.0f && numeb == 0.0f) {
			// coincident
			return false;
		}
		// parallel
		return false;
	}

	float ua = numea / denom;
	float ub = numeb / denom;

	// LINE x LINE intersections
	/*if(result != NULL && 
	   ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)*/

	if(ua >= 0.0f && ub > 0.0f && ub < 1.0f) {
		if(result != NULL) {
			// Get the intersection point.
			result->x = p1.x + ua*(p2.x - p1.x);
			result->y = p1.y + ua*(p2.y - p1.y);
		}

		// intersecting
		return true;
	}

	// not intersecting
	return false;
}

