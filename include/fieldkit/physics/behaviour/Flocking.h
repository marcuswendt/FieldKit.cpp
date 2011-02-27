/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   
 *	Created by David Hoe on 26/05/2010.
 *	Updates by Marcus Wendt on 23/09/2010.
 */

#pragma once

#include "fieldkit/physics/Behaviour.h"

namespace fieldkit { namespace physics {
	
	/************************************************************************/
	/* Base class for all flocking behaviours                               */
	/************************************************************************/
	class FlockingBehaviour: public WeightedBehaviour {
	public:
		FlockingBehaviour(Space* space) : WeightedBehaviour(space) {
			setRange(0.1f);
		};

		void setRange(float value) { range = value; }
		float getRange() { return range; }

		void prepare(float dt);

	protected:
		float range;
		float rangeAbs;
		float rangeAbsSq;
	};


	// Attract - calculate center of neighbours and move towards it
	class FlockAttract : public FlockingBehaviour {
	public:
		FlockAttract(Space* space) : FlockingBehaviour(space){};
		void apply(Particle* Particle);
	};

	//! Align - Calculate average force and move towards it (use velocity if available).
	class FlockAlign : public FlockingBehaviour {
	public:
		FlockAlign(Space* space) : FlockingBehaviour(space){};
		void apply(Particle* Particle);
	};
	
	//! Repel - move away from all neighbours colliding with particle
	class FlockRepel : public FlockingBehaviour {
	public:
		FlockRepel(Space* space) : FlockingBehaviour(space){};
		void apply(Particle* Particle);
	};

} } // namespace fieldkit::physics