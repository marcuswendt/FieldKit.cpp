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
	
	class Force : public WeightedBehaviour {
	public:
		
		Force() : WeightedBehaviour() {
			direction.set(0,0,0);
		};
		
		
		void setDirection(Vec3f value);
		Vec3f getDirection();
		
		void prepare(float dt);
		void apply(Particle* p);
		
	protected:
		Vec3f direction;
		Vec3f force;
	};
	
	// -- Gravity --------------------------------------------------------------
	class Gravity : public Force {
	public:
		Gravity() : Force() {
			direction.set(0, 1, 0);
		}
	};

	// -- Wind -----------------------------------------------------------------
	class Wind : public Force {
	public:
		Wind() : Force() {
			direction.set(1, 0, 0);
		}
	};
} } // namespace fk::physics