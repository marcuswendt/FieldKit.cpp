/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Force.h
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#pragma once

#include "FieldKit.h"
#include "Behaviour.h"
#include "Weighable.h"
#include "Particle.h"

namespace fk { namespace physics {
	
	class Force : public Behaviour, public Weighable {
	public:
		
		Force() : Weighable() {};
		
		void setDirection(Vec3f value) { direction = value.normalized(); }
		Vec3f getDirection() { return direction; }
		
		void apply(Particle* p) { p->force += direction * weight; }
		
	protected:
		Vec3f direction;
	};
	
	// -- Gravity --------------------------------------------------------------
	class Gravity : public Force {
	public:
		Gravity : Force() {
			direction = Vec3f(0, -1, 0);
		}
	};

	// -- Wind -----------------------------------------------------------------
	class Wind : public Force {
	public:
		Gravity : Force() {
			direction = Vec3f(1, 0, 0);
		}
	};
} } // namespace fk::physics