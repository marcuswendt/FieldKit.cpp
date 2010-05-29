/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 29/05/2010.
 */

#include "fieldkit/physics/behaviours/Boundary.h"

using namespace fk::physics;
	
// -- BoxWrap ------------------------------------------------------------------
void BoxWrap::apply(Particle* p) {
	
	bool wrapped = false;
	
	if(p->x < min.x) {
		p->prev.x += max.x - p->x;
		p->x = max.x;
		wrapped = true;
		
	} else if(p->x > max.x) {
		p->prev.x += min.x - p->x;
		p->x = min.x;
		wrapped = true;
	}
	
	if(p->y < min.y) {
		p->prev.y += max.y - p->y;
		p->y = max.y;
		wrapped = true;
		
	} else if(p->y > max.y) {
		p->prev.y += min.y - p->y;
		p->y = min.y;
		wrapped = true;
	}
	
	if(p->z < min.z) {
		p->prev.z += max.z - p->z;
		p->z = max.z;
		wrapped = true;
		
	} else if(p->z > max.z) {
		p->prev.z += min.z - p->z;
		p->z = min.z;
		wrapped = true;
	}
	
	if(wrapped && !preserveMomentum)
		p->clearVelocity();
}
