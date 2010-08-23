/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Behavioural.h"

using namespace fieldkit::physics;

Behavioural::Behavioural()
{
	ownsBehaviours = true;
	ownsConstraints = true;
}

Behavioural::~Behavioural()
{
	if(ownsBehaviours)
		destroyBehaviours();

	if(ownsConstraints)
		destroyConstraints();
}

// Behaviours
void Behavioural::addBehaviour(Behaviour* b) {
	behaviours.push_back(b);
}
	
void Behavioural::removeBehaviour(Behaviour* b) {
	behaviours.remove(b);
}

void Behavioural::destroyBehaviours()
{
	BOOST_FOREACH(Behaviour* b, behaviours) {
		delete b;
	}
	behaviours.clear();
}

// Constraints
void Behavioural::addConstraint(Constraint* c) {
	constraints.push_back(c);
}
	
void Behavioural::removeConstraint(Constraint* c) {
	constraints.remove(c);
}

void Behavioural::destroyConstraints()
{
	BOOST_FOREACH(Constraint* c, constraints) {
		delete c;
	}
	constraints.clear();
}
