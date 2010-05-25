/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Space.cpp
 *	 Created by Marcus Wendt on 23/05/2010.
 */

#include "Space.h"

using namespace fk::math;

namespace fk { namespace physics {
		
	Space::Space() {}
	
	Space::Space(float w, float h, float d) : AABB(w, h, d) {};
	
	void Space::clear() {
		// TODO
	}

	void Space::insert(Spatial* s) {
		// TODO
	}

	void Space::select(fk::math::BoundingVolume* volume) {
		// TODO
	}
	
	Vec3f Space::center() {
		return this->position;
	}

} } // fk::physics