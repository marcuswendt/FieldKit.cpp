/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#include "fieldkit/physics/space/BasicSpace.h"

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

void BasicSpace::insert(SpatialPtr s) 
{
	spatials.push_back(s);
}

void BasicSpace::select(BoundingVolumePtr volume, SpatialList result)
{
	result.clear();
	BOOST_FOREACH(SpatialPtr s, spatials) {
		if(volume->contains(s->getPosition())) {
			result.push_back(s);
		}
	}
}
