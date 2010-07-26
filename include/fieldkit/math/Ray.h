/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 26/07/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"

namespace fieldkit { namespace math {
	
	//! A 2 or 3-dimensional ray
	class Ray2f {
	public:
		Ray2f() {};
		Ray2f(Vec2f origin_, Vec2f direction_) :
			origin(origin_), direction(direction_) {};
		
		~Ray2f() {};
		
		Vec2f getPointAtDistance(float length) {
			return origin + direction * length;
		}
		
		bool findIntersection(Ray2f ray, Vec2f* result);
		
		// Accessors
		Vec2f getOrigin() { return origin; }
		void setOrigin(Vec2f point) { origin = point; }
		
		Vec2f getDirection() { return direction; }
		void setDirection(Vec2f vector) { direction = vector; }
		
	protected:
		Vec2f origin;
		Vec2f direction;
	};
	
} } // namespace fieldkit::math