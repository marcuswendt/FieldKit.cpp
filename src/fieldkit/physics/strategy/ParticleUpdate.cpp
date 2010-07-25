/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 25/07/2010.
 */

#include "fieldkit/physics/strategy/ParticleUpdate.h"

using namespace fieldkit::physics;

//! updates all particles by applying all behaviours and constraints
void ParticleUpdate::apply(Physics* physics) 
{
	float dt = physics->dt;
	
	// prepare behaviours & constraints
	BOOST_FOREACH(BehaviourPtr b, physics->behaviours) {
		b->prepare(dt);
	}
	
	BOOST_FOREACH(ConstraintPtr c, physics->constraints) {
		c->prepare(dt);
	}
	
	// update all particles
	BOOST_FOREACH(ParticlePtr p, physics->particles) {
		if(!p->isAlive) continue;
		
		// apply behaviours
		BOOST_FOREACH(BehaviourPtr b, physics->behaviours) {
			b->apply(p);
		}
		
		// apply constraints
		BOOST_FOREACH(ConstraintPtr c, physics->constraints) {
			c->apply(p);
		}
		
		// update particle
		p->update(dt);
		
		// particle just has died
		if(!p->isAlive) {
			physics->numParticles--;
		}
	}
}