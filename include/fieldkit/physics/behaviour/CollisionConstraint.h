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
#include "fieldkit/math/MathKit.h"

using namespace fieldkit::math;

namespace fieldkit { namespace physics {
	
	class CollisionConstraint : public Constraint {
	public:
		CollisionConstraint() : bouncyness(1.0f) { };
		~CollisionConstraint() {};
		
		void apply(Particle* p);
    
		float getBouncyness() { return bouncyness; }
		void setBouncyness(float b) { bouncyness = b; }
	private:
		float bouncyness;
	};
	
} } // namespace fieldkit::physics