/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Behavioural.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#pragma once

#include "fieldkit/physics/Behaviour.h"
#include "fieldkit/physics/Constraint.h"

#include <list>
using std::list;

namespace fk { namespace physics {
	
	class Behavioural {
	protected:
		list<Behaviour*> behaviours;
		list<Constraint*> constraints;
	
	public:
		Behavioural();
		~Behavioural();

		// Behaviours
		void addBehaviour(Behaviour* b);
		void removeBehaviour(Behaviour* b);

		// Constraints
		void addConstraint(Constraint* c);
		void removeConstraint(Constraint* c);
	};

} } // namespace fk::physics