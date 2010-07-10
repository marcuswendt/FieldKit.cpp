/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

#include "fieldkit/physics/Particle.h"

namespace fieldkit { namespace physics {

	//! typedefs			
	class Space;
	typedef shared_ptr<Space> SpacePtr;
	
	class Behaviour;
	typedef shared_ptr<Behaviour> BehaviourPtr;
	
	// Base class for all physics behaviours
	class Behaviour {
	public:
		SpacePtr space;
		
		Behaviour() {};
		~Behaviour() {};
		
		Behaviour(SpacePtr space) {
			this->space = space;
		};
		
		virtual void prepare(float dt) {};
		virtual void apply(ParticlePtr p) = 0;
	};
	
	// A behaviour with a weight field
	class WeightedBehaviour : public Behaviour {
	public:
		WeightedBehaviour() : weight(1.0f) {}
		WeightedBehaviour(float weight) : weight(weight) {}
		WeightedBehaviour(SpacePtr space) : Behaviour(space), weight(1.0f) {}
		
		void setWeight(float value) { this->weight = value; }
		float getWeight() { return this->weight; }
		
	protected:
		float weight;
	};
	
} } // namespace fieldkit::physics