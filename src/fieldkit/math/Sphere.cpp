/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 06/06/2010.
 */

#include "fieldkit/math/Sphere.h"

using namespace fieldkit::math;

void Sphere::set(Sphere s) {
	center = s.center;
	radius = s.radius;
}

// -- Bounding Volume ----------------------------------------------------------
bool Sphere::contains(Vec3f p) {
	float dx = center.x - p.x;
	float dy = center.y - p.y;
	float dz = center.z - p.z;
	return dx * dx + dy * dy + dz * dz <= radius * radius;
}

bool Sphere::intersects(Sphere s) {
	Vec3f delta = s.center - center;
	float d = delta.length();
	float r1 = radius;
	float r2 = s.radius;
	return d <= r1 + r2 && d >= abs(r1 - r2);
}
