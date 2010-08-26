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
	Vec2f p2 = p2 + direction;
	
	Vec2f p3 = ray.origin;
	Vec2f p4 = p3 + ray.direction;
	
	float denom = ((p4.y - p3.y) * (p2.x - p1.x)) -
				  ((p4.x - p3.x) * (p2.y - p1.y));
	
	float numea = ((p4.x - p3.x) * (p1.y - p3.y)) -
				  ((p4.y - p3.y) * (p1.x - p3.x));
	
	float numeb = ((p2.x - p1.x) * (p1.y - p3.y)) -
				  ((p2.y - p1.y) * (p1.x - p3.x));
	
	if(denom == 0.0f) {
		if(numea == 0.0f && numeb == 0.0f) {
			// coincident
			return false;
		}
		// parallel
		return false;
	}
	
	// Get the intersection point.	
	if(result != NULL) {
		float ua = numea / denom;
		result->x = p1.x + ua*(p2.x - p1.x);
		result->y = p1.y + ua*(p2.y - p1.y);
	}
	
	// intersecting
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

