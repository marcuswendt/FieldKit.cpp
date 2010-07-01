/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/06/2010.
 */

#include "fieldkit/physics/behaviour/Initializer.h"
#include "fieldkit/math/MathKit.h"

using namespace fieldkit::math;
using namespace fieldkit::physics;

Initializer::Initializer() {
	state = 0;
	
	lifeTime = 1000.0f;
	lifeTimeVariance = 0.0f;
	
	size = 1.0;
	sizeVariance = 0.0f;
	
	weight = 1.0;
	weightVariance = 0.0f;
	
	drag = 0.03f;
	dragVariance = 0.0f;
	
	force = Vec3f();
	forceVariance = Vec3f();
	
	lock = false;
	lockChance = 0.0f;
}


void Initializer::apply(ParticlePtr p) {
	p->lifeTime = getVariant(lifeTime, lifeTimeVariance);
	p->size = getVariant(size, sizeVariance);
	p->setWeight( getVariant(weight, weightVariance) );
	p->drag = getVariant(drag, dragVariance);

	p->force.x = getVariant(force.x, forceVariance.x);
	p->force.y = getVariant(force.y, forceVariance.y);
	p->force.z = getVariant(force.z, forceVariance.z);

	if(lock)
		p->isLocked = flipCoin(lockChance);
}


// -- Helpers -----------------------------------------------------------------
float Initializer::getVariant( float value, float variance )
{
	float invariant = value * (1.0f - variance);
	float variant = value * variance * randFloat();
	return invariant + variant;
}


bool Initializer::isPerpetiual()
{
	return lifeTime == Particle::LIFETIME_PERPETUAL;
}


void Initializer::setPerpetual( bool value )
{
	if(value) {
		lifeTime = Particle::LIFETIME_PERPETUAL;
		lifeTimeVariance = 0.0f;
	} else {
		lifeTime = 1000.0f;
	}
}

