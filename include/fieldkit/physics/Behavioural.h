/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Behaviour.h"
#include "fieldkit/physics/Constraint.h"

namespace fieldkit { namespace physics {
	
	class Behavioural {
	public:
		list<Behaviour*> behaviours;
		list<Constraint*> constraints;
		
		Behavioural();
		~Behavioural();

		// Behaviours
		void addBehaviour(Behaviour* b);
		void removeBehaviour(Behaviour* b);
		void destroyBehaviours();

		// Constraints
		void addConstraint(Constraint* c);
		void removeConstraint(Constraint* c);
		void destroyConstraints();

		// Accessors
		void setOwnsBehaviours(bool isOwner) { ownsBehaviours = isOwner; }
		bool getOwnsBehaviours() { return ownsBehaviours; }

		void setOwnsConstraints(bool isOwner) { ownsConstraints = isOwner; }
		bool getOwnsConstraints() { return ownsConstraints; }

	protected:
		bool ownsBehaviours;
		bool ownsConstraints;
	};

} } // namespace fieldkit::physics
