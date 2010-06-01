/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#include "fieldkit/physics/behaviours/Random.h"

#include "cinder/Rand.h"

using namespace ci;
using namespace fk::physics;

void BoxRandom::apply(ParticlePtr p) {
	p->position.x = Rand::randFloat(min.x, max.x);
	p->position.y = Rand::randFloat(min.y, max.y);
	p->position.z = Rand::randFloat(min.z, max.z);
	p->clearVelocity();
}