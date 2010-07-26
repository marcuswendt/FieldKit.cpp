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

	class Ray2f;
	
	//! A 2-dimensional line
	class Line2f {
	public:
		Vec2f p1;
		Vec2f p2;
		
		Line2f() {
			p1 = Vec2f::zero();
			p2 = Vec2f::zero();
		};
		Line2f(Vec2f from, Vec2f to) :
			p1(from), p2(to) {};
		
		~Line2f() {};

		bool findIntersection(Line2f line, Vec2f* result);
		bool findIntersection(Ray2f ray, Vec2f* result);
		
		void set(Vec2f from, Vec2f to);
		
		Vec2f getP1() { return p1; }
		void setP1(Vec2f point) { p1 = point; }
		
		Vec2f getP2() { return p2; }
		void setP2(Vec2f point) { p2 = point; }
		
	protected:

	};
	
} } // namespace fieldkit::math