/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Space.h
 *	 Created by Marcus Wendt on 21/05/2010.
 */

#pragma once

#include "Spatial.h"
#include "BoundingVolume.h"

using namespace fk::math;

namespace fk { namespace physics {
	class Space {
	public:
		Space();
		
		virtual void clear() = 0;
		virtual void insert(Spatial* s) = 0;
		virtual void select(BoundingVolume* volume) = 0;
		
		virtual void setWidth(float value) = 0;
		virtual float getWidth() = 0;
		
		virtual void setHeight(float value) = 0;
		virtual float getHeight() = 0;
		
		virtual void setDepth(float value) = 0;
		virtual float getDepth() = 0;
	};
} } // namespace fk::physics