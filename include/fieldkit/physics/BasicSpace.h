/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   BasicSpace.h
 *	 Created by Marcus Wendt on 23/05/2010.
 */
#pragma once

#include "Space.h"
#include "Spatial.h"
#include "BoundingVolume.h"

namespace fk { namespace physics {
	class BasicSpace : public Space {
	public:
		BasicSpace();
		
		void clear();
		void insert(Spatial* s);
		void select(BoundingVolume* volume);
	};
} } // namespace fk::physics