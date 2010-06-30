/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {
	
	class Force : public WeightedBehaviour {
	public:
		
		Force() : WeightedBehaviour(), direction(Vec3f(0,0,0)) {};
		
		Force(Vec3f dir, float weight) : 
			WeightedBehaviour(weight), direction(dir) {};
		
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
		Gravity(bool upIsPositive=true) : Force() {
			if(upIsPositive)
				direction.set(0, 1, 0);
			else
				direction.set(0, -1, 0);

			setWeight(0.01f);
		}
	};

	// -- Wind -----------------------------------------------------------------
	class Wind : public Force {
	public:
		Wind() : Force() {
			direction.set(1, 0, 0);
			setWeight(0.02f);
		}
	};
} } // namespace fieldkit::physics