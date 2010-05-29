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

#include "fieldkit/Fieldkit.h"
#include "fieldkit/physics/Behaviour.h"
#include "fieldkit/physics/Particle.h"

namespace fk { 
namespace physics {
	
	class FlockAlign : public WeightedBehaviour {
	public:
		void apply(ParticlePtr Particle);
	};
	
	class FlockAttract : public WeightedBehaviour{
	public:
		void apply(ParticlePtr Particle);
	};
	
	class FlockRepel : public WeightedBehaviour {
	public:
		void apply(ParticlePtr Particle);
	};
}

} // namespace fk::physics