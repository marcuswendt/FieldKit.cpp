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

	//! A Sphere that can be used as bounding volume
	class SphereBound : public BoundingVolume {
	public:
		Vec3f center;
		float radius;
		
		//! Constructs a new sphere with the given radius
		SphereBound(float radius_) : center(Vec3f::zero()), radius(radius_) {};

		//! Constructs a new sphere at the given position with the given radius
		SphereBound(Vec3f center_ = Vec3f::zero(), float radius_ = 1.0f) :
			center(center_), radius(radius_) { };

		void set(SphereBound sphere);
		
		// Bounding Volume
		bool contains(Vec3f p);
		bool intersects(SphereBound s);
		
		// Accessors
		void setCenter(Vec3f location) { center = location; };
		Vec3f getCenter() { return center; };
		
		void setRadius(float value) { radius = value; };
		float getRadius() { return radius; };
	};

} } // namespace fieldkit::math