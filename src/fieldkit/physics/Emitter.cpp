/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Emitter.cpp
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Emitter.h"
#include "fieldkit/physics/Physics.h"

namespace fk { 
namespace physics {

Emitter::Emitter(Physics* physics) 
{
	this->physics = physics;		
	rate = 1;
	interval = 0.1;
	max = 100;
	isEnabled = true;
}
	
	void Emitter::update(float dt) {
		if(!isEnabled) return;
		
		// check if its time to start emitting
		time += dt;
		if(time < interval) return;
		
		time = 0;
		
		// emit particles
		int i=0;
		while(i < rate && physics->numParticles() < max) {
			emit(position);
			i += 1;
		}
	}
	
	// emits a single particle and applies the emitter behaviours
	Particle* Emitter::emit(Vec3f location) {
		Particle* p = physics->createParticle();
		
		// set particle to start at the emitters position
		p->init(location);
		
		// apply emitter behaviours
		applyBehaviours(p);
		applyConstraints(p);
		
		return p;
	}
	
	void Emitter::applyBehaviours(Particle* p) {
		BOOST_FOREACH(Behaviour* b, behaviours) {
			b->apply(p);
		}
	}
	
	void Emitter::applyConstraints(Particle* p) {
		BOOST_FOREACH(Constraint* c, constraints) {
			c->apply(p);
		}
	}
} } // namespace fk::physics