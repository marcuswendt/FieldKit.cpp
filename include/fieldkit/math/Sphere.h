/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 06/06/2010.
 */
#pragma once

#include "fieldkit/FieldKit.h"
#include "fieldkit/math/BoundingVolume.h"

namespace fieldkit { namespace math {
	class Sphere : public BoundingVolume {
	public:
		Vec3f center;
		float radius;
		
		Sphere(Vec3f center_ = Vec3f(0,0,0), float radius_ = 1.0f) :
			center(center_), radius(radius_) { };

		void set(Sphere sphere);
		
		// Bounding Volume
		bool contains(Vec3f p);
		bool intersects(Sphere s);
		
		// Accessors
		void setCenter(Vec3f location) { center = location; };
		Vec3f getCenter() { return center; };
		
		void setRadius(float value) { radius = value; };
		float getRadius() { return radius; };
	};
} } // namespace fieldkit::math