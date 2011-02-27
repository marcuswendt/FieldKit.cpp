/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 06/06/2010.
 */

#include "fieldkit/math/SphereBound.h"

using namespace fieldkit;

SphereBound::SphereBound(float radius_) : radius(radius_) 
{
	type = BoundingVolume::BOUNDING_SPHERE;
};

SphereBound::SphereBound(Vec3f center, float radius_) : radius(radius_)
{ 
	type = BoundingVolume::BOUNDING_SPHERE;
	position = center;
}

void SphereBound::set(SphereBound s) {
	position = s.position;
	radius = s.radius;
}

bool SphereBound::contains(Vec3f const& p) {
	float dx = position.x - p.x;
	float dy = position.y - p.y;
	float dz = position.z - p.z;
	return dx * dx + dy * dy + dz * dz <= radius * radius;
}