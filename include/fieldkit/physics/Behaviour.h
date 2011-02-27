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
#include "fieldkit/physics/space/Space.h"

namespace fieldkit { namespace physics {

	// Base class for all physics behaviours
	class Behaviour {
	public:
		Space* space;
		
		Behaviour() {};
		virtual ~Behaviour() {};
		
		Behaviour(Space* space) {
			this->space = space;
		};
		
		virtual void prepare(float dt) {};
		virtual void apply(Particle* p) = 0;
	};
	
	// A behaviour with a weight field
	class WeightedBehaviour : public Behaviour {
	public:
		WeightedBehaviour() : weight(1.0f) {}
		WeightedBehaviour(float weight) : weight(weight) {}
		WeightedBehaviour(Space* space) : Behaviour(space), weight(1.0f) {}
		
		void setWeight(float value) { this->weight = value; }
		float getWeight() { return this->weight; }
		
	protected:
		float weight;
	};
	
} } // namespace fieldkit::physics