/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Behaviour.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#pragma once

#include "Space.h"

namespace fk { namespace physics {

	// forward class declaration
	class Particle;

	class Behaviour {
	public:
		Space* space;
		
		Behaviour() {};
		
		Behaviour(Space* space) {
			this->space = space;
		};
		
		virtual void apply(Particle* p) = 0;
	};
	
} } // namespace fk::physics