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
	class AABB : public Vec3f, BoundingVolume {
	protected:
		Vec3f extent;
		
	public:
		void setWidth(float value);
		float getWidth();
		
		void setHeight(float value);
		float getHeight();
		
		void setDepth(float value);
		float getDepth();
	};
} } // namespace fk::math