/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 25/07/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {

	//! base class for all neighbour update strategies
	class NeighbourUpdate : public PhysicsStrategy {
	public:
		virtual void apply(Physics* physics) = 0;
	};

	class FixedRadiusNeighbourUpdate : public NeighbourUpdate {
	public:
		FixedRadiusNeighbourUpdate() {
			emptySpaceOnUpdate = true;
			setRadius(10.0f);
		};
		
		~FixedRadiusNeighbourUpdate() {};
		
		void apply(Physics* physics);
		
		// Accessors
		void setRadius(float radius) { query.setRadius(radius); }
		void setEmptySpaceOnUpdate(bool enabled) { emptySpaceOnUpdate = enabled; };
		bool getEmptySpaceOnUpdate() { return emptySpaceOnUpdate; };

	protected:
		bool emptySpaceOnUpdate;
		SphereBound query;
	};
	
} } // namespace fieldkit::physics