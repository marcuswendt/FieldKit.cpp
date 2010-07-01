/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#include "fieldkit/physics/space/BasicSpace.h"

#include "fieldkit/physics/space/Spatial.h"

using namespace fieldkit::physics;

BasicSpace::BasicSpace() { 
	clear(); 
};

BasicSpace::~BasicSpace() { 
	//delete &spatials;
	//spatials = NULL;
};

void BasicSpace::clear() 
{
	spatials.clear();
}

void BasicSpace::insert(Spatial* s) 
{
	spatials.push_back(s);
}

void BasicSpace::select(BoundingVolume* volume, list<Spatial*> result)
{
	result.clear();
	BOOST_FOREACH(Spatial* s, spatials) {
		if(volume->contains(s->getSpatialPosition())) {
			result.push_back(s);
		}
	}
}
