/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#include "fieldkit/physics/behaviours/Attractor.h"

using namespace fk::physics;

// -- Point --------------------------------------------------------------------
void AttractorPoint::prepare(float dt) {
	rangeAbs = space->toAbsolute(range);
	rangeAbsSq = rangeAbs * rangeAbs;
}

void AttractorPoint::apply(ParticlePtr p) {

	Vec3f delta = position - *p;
	float distSq = delta.lengthSquared();

	if(distSq < rangeAbsSq) {
		float dist = sqrt(distSq);

		// normalize and inverse proportional weight
		p->force += (delta / dist) * (1.0 - dist/ rangeAbs) * weight;
	}
};
