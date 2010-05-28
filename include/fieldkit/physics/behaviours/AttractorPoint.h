/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Attractor.h
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#pragma once

#include "FieldKit.h"
#include "Behaviour.h"
#include "Weighable.h"
#include "Particle.h"

namespace fk { namespace physics {
	
	class AttractorPoint : public Behaviour, public Weighable {
	public:
		AttractorPoint() : Weighable() {};
		
		Vec3f position;
		virtual void apply(Particle* p) = 0;
	};
	
} } // namespace fk::physics