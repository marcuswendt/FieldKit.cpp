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
void ParticleUpdate::apply(Physics* physics, float dt) 
{	
	vector<Particle*>::iterator pbegin = physics->particles.begin();
	vector<Particle*>::iterator pend = physics->particles.end();

	// apply behaviours
	for (list<Behaviour*>::iterator bit = physics->behaviours.begin(); bit != physics->behaviours.end(); bit++) {
		Behaviour* b = *bit;
		b->prepare(dt);

		for (vector<Particle*>::iterator pit = pbegin; pit != pend; pit++) {
			b->apply(*pit);
		}
	}

	// update all particles
	int numAlive = 0;
	for (vector<Particle*>::iterator pit = pbegin; pit != pend; pit++) {
		Particle* p = *pit;
		p->update(dt);

		// particle just died
		if(p->isAlive)
			numAlive++;
	}
	physics->numParticles = numAlive;

	// apply constraints
	for (int i=0; i<constraintIterations; i++) {
		for (list<Constraint*>::iterator cit = physics->constraints.begin(); cit != physics->constraints.end(); cit++) {
			Constraint* c = *cit;

			if(i==0)
				c->prepare(dt);

			for (vector<Particle*>::iterator pit = pbegin; pit != pend; pit++) {
				c->apply(*pit);
			}
		}
	}

	/*
	// prepare behaviours & constraints
	BOOST_FOREACH(Behaviour* b, physics->behaviours) {
	b->prepare(dt);
	}

	BOOST_FOREACH(Constraint* c, physics->constraints) {
	c->prepare(dt);
	}



	// Parallel For
	#ifdef ENABLE_OPENMP
	vector<Particle*>::iterator it;
	int size = physics->particles.size();

	#pragma omp parallel for
	for(int i=0; i<size; i++) {
		Particle* p = physics->particles[i];

	// Single threaded
	#else
	 for (vector<Particle*>::iterator it = physics->particles.begin(); it != physics->particles.end(); it++) {
		Particle* p = *it;
	#endif

		// Body
		if(p->isAlive) {
			// apply behaviours
			for (list<Behaviour*>::iterator bit = physics->behaviours.begin(); bit != physics->behaviours.end(); bit++) {
				(*bit)->apply(p);
			}

			// apply constraints
			for (int i=0; i<constraintIterations; i++) {
				for (list<Constraint*>::iterator cit = physics->constraints.begin(); cit != physics->constraints.end(); cit++) {
					(*cit)->apply(p);
				}
			}	

			// update particle
			p->update(dt);

			// particle just has died
			if(!p->isAlive) {
				physics->numParticles--;
			}
		} // isAlive
	}
	*/
}