/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Particle.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#pragma once

#include "Behavioural.h"

namespace fk { namespace physics {
	class Particle {
	public:
		int state;
		float age;
		float lifeTime;
		float size;
		bool isLocked;
		Vec3f position;
		Vec3f prev;
		
		Particle();
		virtual ~Particle();
		virtual void update(float dt);
};
	
} } // namespace fk::physics