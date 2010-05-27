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

namespace fk { 
namespace physics {

Physics::Physics(Space* space) {
	this->space = space;
	this->emitter = NULL;
}
	
Physics::~Physics() {
	// TODO clean up particles here?
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
Particle* Physics::createParticle() {
	Particle* p;
		
	if(deadParticles.size() > 0) {
		p = *deadParticles.begin();
		deadParticles.pop_front();
		return p;
	} else {
		p = new Particle();	
	}
		
	// add particle to list
	particles.push_back(p);
		
	return p;
}
	
// updates all particles by applying all behaviours and constraints
void Physics::updateParticles(float dt) {
	BOOST_FOREACH(Particle* p, particles) {
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
		
		// check if particle is still alive, otherwise remove it
		if(!p->isAlive) {
			deadParticles.push_back(p);
			particles.remove(p);
		}
	}
}
	
// -- Springs --------------------------------------------------------------
void Physics::updateSprings() {
	// TODO
}
	
// -- Neighbours -----------------------------------------------------------
void Physics::updateNeighbours() {
	//
	BOOST_FOREACH(Particle *p, particles)
	{
		if(p->neighbourBound)
		{
			p->neighbours.clear();
			space->findSpatialsInVolume(&p->neighbours, p->neighbourBound);
		}
	}
}

} 
} // fk::physics
