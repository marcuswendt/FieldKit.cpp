/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Behavioural.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "Behavioural.h"

namespace fk { namespace physics {
	
	Behavioural::Behavioural() {}
	Behavioural::~Behavioural() {}
	
	void Behavioural::addBehaviour(Behaviour* b) {
		behaviours.push_back(b);
	}
	
	void Behavioural::removeBehaviour(Behaviour* b) {
		behaviours.remove(b);
	}
	
	void Behavioural::addConstraint(Constraint* c) {
		behaviours.push_back(c);
	}
	
	void Behavioural::removeConstraint(Constraint* c) {
		constraints.remove(c);
	}
	
} } // fk::physics

