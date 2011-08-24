/*                                                                           
*      _____  __  _____  __     ____                                   
*     / ___/ / / /____/ / /    /    \   FieldKit
*    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
*   /_/        /____/ /____/ /_____/    http://www.field.io           
*   
*	 Created by Marcus Wendt on 25/07/2010.
*/

#include "fieldkit/physics/strategy/ParticleUpdate.h"
#include "fieldkit/physics/Physics.h"
#include "fieldkit/physics/Particle.h"

using namespace fieldkit::physics;

//! updates all particles by applying all behaviours and constraints
void ParticleUpdate::apply(Physics* physics, float dt) 
{	
    using std::vector;
    using std::list;
    
#ifndef ENABLE_OPENMP
	vector<Particle*>::iterator pbegin = physics->particles.begin();
	vector<Particle*>::iterator pend = physics->particles.end();

    int numAlive = 0;
    for (vector<Particle*>::iterator pit = pbegin; pit != pend; pit++) { 
        Particle* p = *pit;

        // apply behaviours
        for (list<Behaviour*>::iterator bit = physics->behaviours.begin(); bit != physics->behaviours.end(); bit++) {
            Behaviour* b = *bit;
            b->prepare(dt);

            if(p->isAlive)
                b->apply(p);
        }

        // update all particles

		if(!p->isAlive) 
			continue;
		p->update(dt);
		numAlive++;
	
        physics->numParticles = numAlive;

        // apply constraints
        for (int i=0; i<constraintIterations; i++) {
            for (list<Constraint*>::iterator cit = physics->constraints.begin(); cit != physics->constraints.end(); cit++) {
                Constraint* c = *cit;

                if(i==0)
                    c->prepare(dt);

				if(p->isAlive)
					c->apply(p);
			}
		}
	}
#else
	//
	// NOTE: OpenMP support is still quite experimental - can cause crashes!
	//
	int psize = physics->particles.size();

	// apply behaviours
	for (list<Behaviour*>::iterator bit = physics->behaviours.begin(); bit != physics->behaviours.end(); bit++) {
		Behaviour* b = *bit;
		b->prepare(dt);

		#pragma omp parallel for
		for(int i=0; i < psize; i++) {
			Particle* p = physics->particles[i];
			if(p->isAlive)
				b->apply(p);
		}
	}

	// update all particles
	int numAlive = 0;
	//#pragma omp parallel for
	for(int i=0; i < psize; i++) {
		Particle* p = physics->particles[i];
		if(!p->isAlive) 
			continue;
		p->update(dt);
		numAlive++;
	}
	physics->numParticles = numAlive;

	// apply constraints
	for (int i=0; i<constraintIterations; i++) {
		for (list<Constraint*>::iterator cit = physics->constraints.begin(); cit != physics->constraints.end(); cit++) {
			Constraint* c = *cit;

			if(i==0) 
				c->prepare(dt);

			#pragma omp parallel for
			for(int i=0; i < psize; i++) {
				Particle* p = physics->particles[i];
				if(p->isAlive)
					c->apply(p);
			}
		}
	}
#endif
}