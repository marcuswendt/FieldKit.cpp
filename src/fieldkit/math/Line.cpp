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
	
	if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f) {
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


////! Calculates the intersection between a ray and a line, also checks for parallel lines.
//bool Line2f::findIntersection(Ray2f const& ray, Vec2f* result)
//{
//	return ray.findIntersection(this, result);
//}
