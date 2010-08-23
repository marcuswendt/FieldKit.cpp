/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 29/05/2010.
 */

#include "fieldkit/physics/behaviour/Boundary.h"

using namespace fieldkit::physics;
	
void BoxWrap::apply(Particle* p) {
	
	bool wrapped = false;
	
	if(p->position.x < min.x) {
		p->prev.x += max.x - p->position.x;
		p->position.x = max.x;
		wrapped = true;
		
	} else if(p->position.x > max.x) {
		p->prev.x += min.x - p->position.x;
		p->position.x = min.x;
		wrapped = true;
	}
	
	if(p->position.y < min.y) {
		p->prev.y += max.y - p->position.y;
		p->position.y = max.y;
		wrapped = true;
		
	} else if(p->position.y > max.y) {
		p->prev.y += min.y - p->position.y;
		p->position.y = min.y;
		wrapped = true;
	}
	
	if(p->position.z < min.z) {
		p->prev.z += max.z - p->position.z;
		p->position.z = max.z;
		wrapped = true;
		
	} else if(p->position.z > max.z) {
		p->prev.z += min.z - p->position.z;
		p->position.z = min.z;
		wrapped = true;
	}
	
	if(wrapped && !preserveMomentum)
		p->clearVelocity();
}
