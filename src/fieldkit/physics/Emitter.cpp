/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Emitter.cpp
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Emitter.h"
#include "fieldkit/physics/Physics.h"

using namespace fieldkit::physics;

Emitter::Emitter(Physics* physics) 
{
	this->physics = physics;		
	rate = 1;
	interval = 0.1f;
	max = 100;
	isEnabled = true;
	time = 0;
}

void Emitter::setMax(int value) {
	max = value;
	physics->allocParticles(value);
}

void Emitter::update(float dt) {
	if(!isEnabled) return;
	
	// prepare behaviours & constraints
	BOOST_FOREACH(Behaviour* b, behaviours) {
		b->prepare(dt);
	}
	BOOST_FOREACH(Constraint* c, constraints) {
		c->prepare(dt);
	}
	
	// check if its time to start emitting
	time += dt;
	if(time < interval) return;
	
	time = 0;
	
	// emit particles
	int i=0;
	while(i < rate && physics->getNumParticles() < max) {
		emit(position);
		i += 1;
	}
}

// emits a single particle and applies the emitter behaviours
Particle* Emitter::emit(Vec3f const& location) {
	if(physics->getNumParticles() == physics->getNumAllocated()) {
		logger() << "Emitter::emit cannot emit more particles - allocate more particles first." << endl;
		return NULL;
	}
	Particle* p = physics->createParticle();
	
	// set particle to start at the emitters position
	p->init(location);
	
	// apply emitter behaviours
	BOOST_FOREACH(Behaviour* b, behaviours) {
		b->apply(p);
	}

	// apply emitter constraints
	BOOST_FOREACH(Constraint* c, constraints) {
		c->apply(p);
	}

	return p;
}
