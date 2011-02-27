/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#include "fieldkit/physics/behaviour/Random.h"

using namespace fieldkit::physics;

void BoxRandom::apply(Particle* p) {
	p->position.x = randFloat(min.x, max.x);
	p->position.y = randFloat(min.y, max.y);
	p->position.z = randFloat(min.z, max.z);
	p->clearVelocity();
}