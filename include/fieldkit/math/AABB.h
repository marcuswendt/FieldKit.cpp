/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   AABB.h
 *	 Created by Marcus Wendt on 24/05/2010.
 */
#pragma once

#include "BoundingVolume.h"

namespace fk { namespace math {
	class AABB : public BoundingVolume {
	public:
		Vec3f min;
		Vec3f max;
		Vec3f extent;  // half sized dimensions (halfw,halfh,halfdepth)
		
		AABB();
		AABB(float w, float h, float d);
		
		bool contains(Vec3f p);
		
		void width(float value);
		float width();
		
		void height(float value);
		float height();
		
		void depth(float value);
		float depth();
		
		void dimension(float w, float h, float d);
		Vec3f dimension();
		
	protected:
		void updateBounds();
	};
} } // namespace fk::math