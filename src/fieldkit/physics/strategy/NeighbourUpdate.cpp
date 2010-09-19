/*                                                                           
*      _____  __  _____  __     ____                                   
*     / ___/ / / /____/ / /    /    \   FieldKit
*    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
*   /_/        /____/ /____/ /_____/    http://www.field.io           
*   
*	 Created by Marcus Wendt on 25/07/2010.
*/

#include "fieldkit/physics/strategy/NeighbourUpdate.h"

using namespace fieldkit::physics;

void FixedRadiusNeighbourUpdate::apply(Physics* physics) 
{
	if(emptySpaceOnUpdate) 
		physics->space->clear();

	for (vector<Particle*>::iterator it = physics->particles.begin(); it != physics->particles.end(); it++) {
		Particle* p = *it;
		if(p->isAlive)
			physics->space->insert(p);
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
			query.position = p->position;
			physics->space->select(&query, p->getNeighbours());
		}
	} 
}