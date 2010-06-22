/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Physics.cpp
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#include "fieldkit/physics/Physics.h"

using namespace fieldkit::physics;

Physics::Physics(Space* space) {
	this->space = space;
	emitter = NULL;
	numParticles = 0;
}

Physics::~Physics() {
	particles.clear();
	springs.clear();
}

void Physics::update(float dt) {
	if(emitter != NULL)
		emitter->update(dt);
	
	updateParticles(dt);
	updateSprings();
	updateNeighbours();
}

// -- Particles ------------------------------------------------------------
// check if we still have a dead particle that we can reuse, otherwise create a new one
ParticlePtr Physics::createParticle() {
	numParticles++;
	for(ParticlePtr pIt = particles.begin(); pIt != particles.end();) {
		if(!pIt->isAlive) return pIt;
		pIt++;
	}
	printf("WARNING: running out of particles\n");
	// FIXME
	return particles.begin();
}

// allocates a bunch of new particles
void Physics::allocParticles(int count) {
	particles.reserve(count);
	//while(particles.size() < count) {
	for(int i =0; i< count ;i++){
		particles.push_back( Particle());
	}
	//}
}

// updates all particles by applying all behaviours and constraints
void Physics::updateParticles(float dt) {
	// prepare behaviours & constraints

	BOOST_FOREACH(Behaviour* b, behaviours) {
		b->prepare(dt);
	}
	
	BOOST_FOREACH(Constraint* c, constraints) {
		c->prepare(dt);
	}
	
	// update all particles
	for(ParticlePtr p = particles.begin(); p != particles.end(); p++) {
		if(!p->isAlive) continue;
		
		// apply behaviours
		BOOST_FOREACH(Behaviour* b, behaviours) {
			b->apply(p);
		}
		
		// apply constraints
		BOOST_FOREACH(Constraint* c, constraints) {
			c->apply(p);
		}
		
		// update particle
		p->update(dt);
		
		// particle just has died
		if(!p->isAlive) {
			numParticles--;
		}
	}
}


// -- Springs --------------------------------------------------------------
void Physics::addSpring(Spring* spring) {
	springs.push_back(spring);
}

void Physics::removeSpring(Spring* spring) {
	// TODO
//	springs.erase(spring);
}

// updates all spring connections based on new particle positions
void Physics::updateSprings() {
	// update all springs
	BOOST_FOREACH(Spring* s, springs) {
		s->update();
		
		// apply constraints after spring update
		BOOST_FOREACH(Constraint* c, constraints) {
			c->apply(s->a);
			c->apply(s->b);
		}			
	}		
}

// -- Neighbours -----------------------------------------------------------
void Physics::updateNeighbours() {
	/* 
	BOOST_FOREACH(Particle p, particles) {
		if(p.neighbourBound) {
			p.neighbours.clear();
			space->findSpatialsInVolume(&p.neighbours, p.neighbourBound);
		}
	}*/
}
