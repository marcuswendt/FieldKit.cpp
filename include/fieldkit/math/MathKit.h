/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 28/06/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"

// Global math helper methods
namespace fieldkit { namespace math {

	// Carmack's inverse square root function
	float invSqrt(float x);
	
	float randFloat();
	float randFloat(float range);
	float randFloat(float from, float to);

	Vec3f randVec3f();
	Vec3f randVec3f(float range);
	Vec3f randVec3f(float from, float to);

	Vec3f randNormal();

	bool flipCoin(float chance);
	
} } // namespace fieldkit::math


// Bounding Volumes
#include "fieldkit/math/BoundingVolume.h"
#include "fieldkit/math/AABB.h"
#include "fieldkit/math/SphereBound.h"
