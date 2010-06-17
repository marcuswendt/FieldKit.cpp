/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#include "fieldkit/physics/behaviours/PlaneConstraint.h"

using namespace fk::physics;

void PlaneConstraint::apply(ParticlePtr p) {
	p->position[axis] = constraint;
}


void FloorConstraint::apply(ParticlePtr p) {
	if(p->position[axis] < height) {
		float speed = p->getSpeed();
		p->position[axis] = height;
		p->clearVelocity();
		p->prev[axis] = -speed * bouncyness;
	}
}

void fk::physics::FloorConstraint::setBouncyness(float value)
{
	bouncyness = value;
}
