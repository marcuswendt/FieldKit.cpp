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

	BOOST_FOREACH(Particle* p, physics->particles) {
		if(p->isAlive)
			physics->space->insert(p);
	}

	BOOST_FOREACH(Particle* p, physics->particles) {
		if(p->isAlive) {
			query->position = p->position;
			physics->space->select(query, p->getNeighbours());
		}
	}

}