/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 25/07/2010.
 */

#pragma once

#include "fieldkit/physics/strategy/PhysicsStrategy.h"

namespace fieldkit { namespace physics {
	
	class SpringUpdate : public PhysicsStrategy {
	public:
		SpringUpdate() {};
		~SpringUpdate() {};
		void apply(Physics* physics);
	};
	
} } // namespace fieldkit::physics