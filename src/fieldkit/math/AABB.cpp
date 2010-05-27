/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   AABB.cpp
 *	 Created by Marcus Wendt on 24/05/2010.
 */

#include "fieldkit/math/AABB.h"

namespace fk { 
namespace math {
	
AABB::AABB() {}
	
AABB::AABB(float w, float h, float d) : BoundingVolume() {
	position = Vec3f(w/2.0f, h/2.0f, d/2.0f);
	dimension(w, h, d);
}
	
bool AABB::contains(Vec3f p) {
	if(p.x < min.x || p.x > max.x) return false;
	if(p.y < min.y || p.y > max.y) return false;
	if(p.z < min.z || p.z > max.z) return false;
	return true;
}
	
void AABB::width(float value) {
	extent.x = value * 0.5f;
	updateBounds();
}
	
float AABB::width() {
	return extent.x * 2.0f;
}

void AABB::height(float value) {
	extent.y = value * 0.5f;
	updateBounds();
}
	
float AABB::height() {
	return extent.y * 2.0f;
}

void AABB::depth(float value) {
	extent.z = value * 0.5f;
	updateBounds();
}
	
float AABB::depth() {
	return extent.z * 2.0f;
}
	
void AABB::dimension(float w, float h, float d) {
	extent.x = w * 0.5f; 
	extent.y = h * 0.5f;
	extent.z = d * 0.5f;
	updateBounds();
}
	
Vec3f AABB::dimension() {
	return extent * 2.0f;
}
	
void AABB::updateBounds() {
	min = position - extent;
	max = position + extent;
}

} 
} // namespace fk::math