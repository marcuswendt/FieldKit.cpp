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
	class AABB : BoundingVolume {
	public:
		Vec3f min;
		Vec3f max;
		Vec3f extent;
		
		AABB();
		AABB(float w, float h, float d);
		
		bool contains(Vec3f p);
		
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
} } // namespace fk::math