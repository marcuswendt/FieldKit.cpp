/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#include "fieldkit/physics/behaviour/PlaneConstraint.h"

using namespace fieldkit::physics;

void PlaneConstraint::apply(ParticlePtr p) {
	p->position[axis] = constraint;
}


void WallConstraint::apply(ParticlePtr p) {
	if(p->position[axis] > height) {
		float velocity = p->getVelocity()[axis];
		p->position[axis] = height;
		p->prev[axis] = height - velocity * bouncyness;
	}
}

