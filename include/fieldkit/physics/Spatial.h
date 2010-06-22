/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Spatial.h
 *	 Created by Marcus Wendt on 24/05/2010.
 */
#pragma once

#include "fieldkit/math/BoundingVolume.h"
using namespace fieldkit::math;
#include "fieldkit/FieldKit.h"

namespace fieldkit { 
namespace physics {

enum SPATIAL_TYPE{
	SPATIAL_TYPE_PARTICLE
};

class Spatial
{
public:
	
	Spatial(){};
	virtual Vec3f *getSpatialPosition(){ return 0; };
};

} 
} // namespace fieldkit::physics