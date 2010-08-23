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
		CollisionConstraint() {};
		~CollisionConstraint() {};
		
		void apply(Particle* p);
	};
	
} } // namespace fieldkit::physics