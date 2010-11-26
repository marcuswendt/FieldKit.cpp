/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#include "fieldkit/physics/behaviour/Attractor.h"

using namespace fieldkit::physics;

// -- Point --------------------------------------------------------------------
void AttractorPoint::setRange(float value) 
{
	range = value;
	rangeAbs = space->toAbsolute(range);
	rangeAbsSq = rangeAbs * rangeAbs;
}

void AttractorPoint::apply(Particle* p) 
{
	Vec3f delta = position - p->position;
	float distSq = delta.lengthSquared();

	if(distSq < rangeAbsSq) {
		float dist = sqrtf(distSq);

		// normalize and inverse proportional weight
		p->force += (delta / dist) * (1.0f - dist/ rangeAbs) * weight;
	}
}
