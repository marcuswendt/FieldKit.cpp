/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   
 *	 Created by David Hoe on 26/05/2010.
 */
#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fk { namespace physics {
	
	class FlockBaseBehaviour: public WeightedBehaviour{
	public:
		FlockBaseBehaviour(Space* space) : WeightedBehaviour(space) {
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

	class FlockAlign : public FlockBaseBehaviour {
	public:
		FlockAlign(Space* space) : FlockBaseBehaviour(space){};
		void apply(ParticlePtr Particle);
	};
	
	class FlockAttract : public FlockBaseBehaviour{
	public:
		FlockAttract(Space* space) : FlockBaseBehaviour(space){};
		void apply(ParticlePtr Particle);
	};
	
	class FlockRepel : public FlockBaseBehaviour {
	public:
		FlockRepel(Space* space) : FlockBaseBehaviour(space){};
		void apply(ParticlePtr Particle);
	};

} } // namespace fk::physics