/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 24/05/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"
#include "fieldkit/math/BoundingVolume.h"

namespace fieldkit { namespace math {
	
	//! An axis aligned bounding box
	class AABB : public BoundingVolume {
	public:
		Vec3f min;
		Vec3f max;
		Vec3f extent;  // half sized dimensions (halfw,halfh,halfdepth)
		
		AABB();
		AABB(float w, float h, float d);
		AABB(Vec3f const& min, Vec3f const& max);
		
		// Bounding Volume
		bool contains(Vec3f const& point);
		
		//! Grows / shrinks the box by the given amount
		void expand(Vec3f const& amount);

		// Accessors
		void set(Vec3f const& min, Vec3f const& max);
		void set(AABB const& box);
		void set(AABB* box);
		
		void setWidth(float value);
		float getWidth();
		
		void setHeight(float value);
		float getHeight();
		
		void setDepth(float value);
		float getDepth();
		
		void setDimension(float w, float h, float d);
		Vec3f getDimension();
		
	protected:
		void updateBounds();
		
	};
	
} } // namespace fieldkit::math