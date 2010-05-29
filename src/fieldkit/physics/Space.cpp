/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/05/2010.
 */

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Space.h"

using namespace fk::math;

namespace fk { 
namespace physics {
		
Space::Space() : AABB() {}

Space::Space(float w, float h, float d) : AABB(w, h, d) {};

void Space::clear() 
{
	spatials.clear();
}

void Space::insert(Spatial* s) 
{
	spatials.push_back(s);
}

// find spatials with the volume
void Space::findSpatialsInVolume(list<void *> *resultlist, fk::math::BoundingVolume* volume) 
{
	resultlist->clear();
	ci::Vec3f *pos;
	BOOST_FOREACH(Spatial *obj, spatials)
	{
		pos = obj->getSpatialPosition();
		if(volume->contains(*pos))
		{
			resultlist->push_back(obj);
		}
	}
}

void Space::select(BoundingVolume* volume)
{
	
}

Vec3f Space::getCenter() {
	return this->position;
}

} 
} // fk::physics