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

	const float randFloat();
	const float randFloat(float range);
	const float randFloat(float from, float to);

	const Vec3f randVec3f();
	const Vec3f randVec3f(float range);
	const Vec3f randVec3f(float from, float to);

	const Vec3f randNormal();

	const bool flipCoin(float chance);

} } // namespace fieldkit::math


// Bounding Volumes
#include "fieldkit/math/BoundingVolume.h"
#include "fieldkit/math/AABB.h"
#include "fieldkit/math/Sphere.h"