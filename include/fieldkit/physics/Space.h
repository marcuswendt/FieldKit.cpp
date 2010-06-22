/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 21/05/2010.
 */

#pragma once

#include "fieldkit/math/AABB.h"
#include "fieldkit/math/BoundingVolume.h"
#include "fieldkit/physics/Spatial.h"
#include <list>

using namespace fieldkit::math;

namespace fieldkit { 
namespace physics {

class Space : public AABB 
{
public:
	list<Spatial *>spatials;
		
	Space() {}
	Space(float w, float h, float d) : AABB(w, h, d) {};
	Space(Vec3f min, Vec3f max) : AABB(min, max) {};
		
	virtual void clear();
	virtual void insert(Spatial* s);
	virtual void findSpatialsInVolume(list<void *> *resultlist, fk::math::BoundingVolume* volume); 
	virtual void select(BoundingVolume* volume); 

	virtual Vec3f getCenter();
	
	float toAbsolute(float value);
	float toRelative(float value);
};

} 
} // namespace fieldkit::physics