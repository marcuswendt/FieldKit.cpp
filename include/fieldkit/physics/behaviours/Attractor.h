/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Particle.h"
#include "fieldkit/physics/Behaviour.h"

namespace fk { namespace physics {
	
	class AttractorPoint : public WeightedBehaviour {
	public:
		AttractorPoint() : WeightedBehaviour() {};
		
		Vec3f position;
		virtual void apply(ParticlePtr p) = 0;
	};
	
} } // namespace fk::physics