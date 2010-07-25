/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 25/07/2010.
 */

#include "fieldkit/physics/strategy/SpringUpdate.h"

using namespace fieldkit::physics;

//! updates all spring connections based on new particle positions
void SpringUpdate::apply(Physics* physics) 
{
	BOOST_FOREACH(SpringPtr s, physics->springs) {
		s->update();
		
		// apply constraints after spring update
		BOOST_FOREACH(ConstraintPtr c, physics->constraints) {
			c->apply(s->a);
			c->apply(s->b);
		}			
	}
}