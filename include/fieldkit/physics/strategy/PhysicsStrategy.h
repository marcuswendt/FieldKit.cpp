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
	
	// forward
	class Physics;
	
	//! base class for all physics strategies
	class PhysicsStrategy {
	public:
		PhysicsStrategy() {};
		~PhysicsStrategy() {};
		
		virtual void apply(Physics* physics) = 0;
	};
	
	typedef shared_ptr<PhysicsStrategy> PhysicsStrategyPtr;
	
} } // namespace fieldkit::physics