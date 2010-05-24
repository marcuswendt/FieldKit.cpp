/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Physics.cpp
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#include "Physics.h"

namespace fk { namespace physics {

	Physics::Physics() {}
	
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
	void Physics::updateParticles(float dt) {
		BOOST_FOREACH(Particle* p, particles) {
			p->update(dt);
		}
	}
	
	void Physics::addParticle(Particle* p) { 
		particles.push_back(p);
	}

	void Physics::removeParticle(Particle* p) { 
		particles.remove(p);
	}
	
	// -- Springs --------------------------------------------------------------
	void Physics::updateSprings() {
		// TODO
	}
	
	// -- Neighbours -----------------------------------------------------------
	void Physics::updateNeighbours() {
		// TODO
	}
} } // fk::physics
