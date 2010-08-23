/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/08/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {
	
	// FWD
	class Physics;
	
	//! Creates new particle instances
	class ParticleAllocator : public PhysicsStrategy {
	public:
		ParticleAllocator() {};
		~ParticleAllocator() {};
		
		void apply(Physics* physics);
	};

} } // namespace fieldkit::physics