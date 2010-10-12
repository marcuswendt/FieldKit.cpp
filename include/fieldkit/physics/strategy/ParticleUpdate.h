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

namespace fieldkit { namespace physics {
	
	class ParticleUpdate : public PhysicsStrategy {
	public:
		ParticleUpdate() {
			constraintIterations = 1;
		};
		~ParticleUpdate() {};
		
		void apply(Physics* physics, float dt);
		
		// Accessors
		void setConstraintIterations(int iterations) { constraintIterations = iterations; };
		int getConstraintIterations() { return constraintIterations; };
		
	protected:
		int constraintIterations;
	};
} } // namespace fieldkit::physics