/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 06/06/2010.
 */

#include "fieldkit/physics/behaviour/SphereConstraint.h"

using namespace fieldkit::physics;

void SphereConstraint::apply(ParticlePtr p) {
	bool isInside = contains(p->position);
	if((isBoundingSphere && !isInside) || (!isBoundingSphere && isInside)) {
		p->position.set(
			((p->position - position).normalized() *= radius) += position
		);
	}	
}